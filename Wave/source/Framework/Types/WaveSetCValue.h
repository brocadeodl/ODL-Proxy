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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian / Kundan Sinha                       *
 ***************************************************************************/

#ifndef WAVESETCVALUE_H_
#define WAVESETCVALUE_H_


#include <string.h>
#include <string>
#include <vector>
#include "Framework/Types/WaveCValue.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Types/Types.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Types/Date.h"
#include "Framework/Types/IpAddressNetworkMask.h"
#include "Framework/Types/BitMap.h"
#include "Framework/Types/HostUC.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Types/SnmpObjectId.h"
#include "Framework/Types/IpAddress.h"
#include "Framework/Types/Time.h"
#include "Framework/Types/Decimal64.h"
#include "Framework/Types/LargeObject.h"


#ifdef __cplusplus
extern "C"
{
#endif
    
static inline void wave_delete_cvalue (WaveCValue *pCValue)
{
    switch (pCValue->type)
    {
        case WaveNs::WAVE_CVALUE_STRING:
        case WaveNs::WAVE_CVALUE_MAC_ADDRESS:
        case WaveNs::WAVE_CVALUE_MAC_ADDRESS2:
        case WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME:
            free (pCValue->value.String);
            pCValue->value.String = NULL;
            break;
        case WaveNs::WAVE_CVALUE_STRING_VECTOR:
        case WaveNs::WAVE_CVALUE_MAC_ADDRESS_VECTOR:
        case WaveNs::WAVE_CVALUE_MAC_ADDRESS2_VECTOR:
        case WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME_VECTOR:
        case WaveNs::WAVE_CVALUE_MAC_RANGE:
            {
            	uint32_t i ;

            	for(i = 0; i < pCValue->value.string_vector_value.size; i++) {
                	free (pCValue->value.string_vector_value.values[i]);
            	}
            	delete[] (pCValue->value.string_vector_value.values);
                pCValue->value.string_vector_value.values = NULL;
            }
            break;
        case WaveNs::WAVE_CVALUE_SI32_VECTOR:
            delete[] (pCValue->value.si32_vector_value.values);
            pCValue->value.si32_vector_value.values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_UI32_VECTOR:
            delete[] (pCValue->value.ui32_vector_value.values);
            pCValue->value.ui32_vector_value.values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_SI64_VECTOR:
            delete[] (pCValue->value.si64_vector_value.values);
            pCValue->value.si64_vector_value.values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_UI64_VECTOR:
            delete[] (pCValue->value.ui64_vector_value.values);
            pCValue->value.ui64_vector_value.values = NULL;
            break;
		case WaveNs::WAVE_CVALUE_BITMAP:
		    delete[] (pCValue->value.bitmap_value.bit_blocks);
            pCValue->value.bitmap_value.bit_blocks = NULL;  			
			break;
        case WaveNs::WAVE_CVALUE_DATE_VECTOR:
            delete[] (pCValue->value.date_vector_value.values);
            pCValue->value.date_vector_value.values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_DATE_TIME_VECTOR:
            delete[] (pCValue->value.datetime_vector_value.datetime_values);
            pCValue->value.datetime_vector_value.datetime_values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_DECIMAL_64_VECTOR:
            delete[] (pCValue->value.decimal64_vector_value.decimal_64_values);
            pCValue->value.datetime_vector_value.datetime_values = NULL;
            break;
        case WaveNs::WAVE_CVALUE_HOST:
            free (pCValue->value.host_value.address);
            pCValue->value.host_value.address = NULL;
            break;
        case WaveNs::WAVE_CVALUE_IPADDRESS_VECTOR: 
            {
                uint32_t i ;

                for(i = 0; i < pCValue->value.ipaddress_vector_value.size; i++)
                {
                    free (pCValue->value.ipaddress_vector_value.values[i].ip_address);
                }  
            }
            delete[] (pCValue->value.ipaddress_vector_value.values);
            pCValue->value.ipaddress_vector_value.values = NULL;
            break;
            case WaveNs::WAVE_CVALUE_TIME_VECTOR:
            delete[] (pCValue->value.time_vector_value.values);
            pCValue->value.time_vector_value.values = NULL; 
            break;
        case WaveNs::WAVE_CVALUE_LARGEOBJECT:
            free (pCValue->value.large_object_value.byte_blocks);
            pCValue->value.large_object_value.byte_blocks = NULL;
            break;

        default:
            break;
    }
}


static inline void wave_cvalue_set_bool (WaveCValue *cvalue, const bool value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_BOOL;
    cvalue->value.ui32 = value;
}


static inline void wave_cvalue_set_ui32 (WaveCValue *cvalue, const uint32_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_UI32;
    cvalue->value.ui32 = value;
}


static inline void wave_cvalue_set_si32 (WaveCValue *cvalue, const int32_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_SI32;
    cvalue->value.si32 = value;
}


static inline void wave_cvalue_set_si64 (WaveCValue *cvalue, const int64_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_SI64;
    cvalue->value.si64 = value;
}

static inline void wave_cvalue_set_ui64 (WaveCValue *cvalue, const uint64_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_UI64;
    cvalue->value.ui64 = value;
}

static inline void wave_cvalue_set_ui16 (WaveCValue *cvalue, const uint16_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_UI16;
    cvalue->value.ui16 = value;
}

static inline void wave_cvalue_set_si16 (WaveCValue *cvalue, const int16_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_SI16;
    cvalue->value.si16 = value;
}



static inline void wave_cvalue_set_ui8 (WaveCValue *cvalue, const uint8_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_UI8;
    cvalue->value.ui8 = value;
}



static inline void wave_cvalue_set_si8 (WaveCValue *cvalue, const int8_t value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_SI8;
    cvalue->value.si8 = value;
}


static inline void wave_cvalue_set_char (WaveCValue *cvalue, const char value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_CHAR;
    cvalue->value.charvalue = value;
}


static inline void wave_cvalue_set_string (WaveCValue *cvalue, const string value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_STRING;
    
    cvalue->value.String = strdup (value.c_str());
    WaveNs::prismAssert (NULL != cvalue->value.String, __FILE__, __LINE__);
}

static inline void wave_cvalue_set_string_vector (WaveCValue *cvalue, const vector<string> value)
{
    wave_string_vector str_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    char **strvalues = new char*[len];
    WaveNs::prismAssert (NULL != strvalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        strvalues[i]= strdup (value[i].c_str());
        WaveNs::prismAssert (NULL != strvalues[i], __FILE__, __LINE__);
    }

    str_vect.size = len;
    str_vect.values = strvalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_STRING_VECTOR;
    cvalue->value.string_vector_value = str_vect;
}


static inline void wave_cvalue_set_si32_vector (WaveCValue *cvalue, const vector<WaveNs::SI32> value)
{
    wave_si32_vector si32_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    int32_t *sivalues = new int32_t[len];
    WaveNs::prismAssert (NULL != sivalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        sivalues[i] = value[i];
    }

    si32_vect.size = len;
    si32_vect.values = sivalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_SI32_VECTOR;
    cvalue->value.si32_vector_value = si32_vect;
}


static inline void wave_cvalue_set_bool_vector (WaveCValue *cvalue, const vector<bool> value)
{
    wave_ui32_vector ui32_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    uint32_t *uivalues = new uint32_t[len];
    WaveNs::prismAssert (NULL != uivalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        uivalues[i] = value[i];
    }

    ui32_vect.size = len;
    ui32_vect.values = uivalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_BOOL_VECTOR;
    cvalue->value.ui32_vector_value = ui32_vect;
}


static inline void wave_cvalue_set_ui32_vector (WaveCValue *cvalue, const vector<WaveNs::UI32> value)
{
    wave_ui32_vector ui32_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();

    uint32_t *uivalues = new uint32_t[len];
    WaveNs::prismAssert (NULL != uivalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        uivalues[i] = value[i];
    }

    ui32_vect.size = len;
    ui32_vect.values = uivalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_UI32_VECTOR;
    cvalue->value.ui32_vector_value = ui32_vect;
}

static inline void wave_cvalue_set_si64_vector (WaveCValue *cvalue, const vector<WaveNs::SI64> value)
{
    wave_si64_vector si64_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();

    int64_t *sivalues = new int64_t[len];
    WaveNs::prismAssert (NULL != sivalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        sivalues[i] = value[i];
    }

    si64_vect.size = len;
    si64_vect.values = sivalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_SI64_VECTOR;
    cvalue->value.si64_vector_value = si64_vect;
}


static inline void wave_cvalue_set_ui64_vector (WaveCValue *cvalue, const vector<WaveNs::UI64> value)
{
    wave_ui64_vector ui64_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();

    uint64_t *uivalues = new uint64_t[len];
    WaveNs::prismAssert (NULL != uivalues, __FILE__, __LINE__);

    for(i = 0; i < len; i++)
    {
        uivalues[i] = value[i];
    }

    ui64_vect.size = len;
    ui64_vect.values = uivalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_UI64_VECTOR;
    cvalue->value.ui64_vector_value = ui64_vect;
}


static inline void wave_cvalue_set_bitmap (WaveCValue *cvalue, const WaveNs::BitMap value)
{
    wave_bitmap bit_map;
    
    bit_map.number_of_bits = value.getNumberOfBits ();
    bit_map.number_of_bitblocks = ((bit_map.number_of_bits + 63)/ 64) ;

    bit_map.bit_blocks = new uint64_t[bit_map.number_of_bitblocks];
    WaveNs::prismAssert (NULL != bit_map.bit_blocks, __FILE__, __LINE__);
    
    cvalue->type = WaveNs::WAVE_CVALUE_BITMAP;
    cvalue->value.bitmap_value = bit_map;
}


static inline void wave_cvalue_set_date (WaveCValue *cvalue, const WaveNs::Date value)
{
    wave_date date_val;

    date_val.year = value.getYear ();
    date_val.month = value.getMonth ();
    date_val.day = value.getDay ();
    date_val.timezone_hours = value.getTimezoneHours ();
    date_val.timezone_minutes = value.getTimezoneMinutes ();
    
    cvalue->type = WaveNs::WAVE_CVALUE_DATE;
    cvalue->value.date_value = date_val;
}


static inline void wave_cvalue_set_date_vector (WaveCValue *cvalue, const vector<WaveNs::Date> value)
{
    wave_date_vector date_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    wave_date* date_ptr = new wave_date[len];
    WaveNs::prismAssert (NULL != date_ptr, __FILE__, __LINE__);

    for(i = 0; i < len; i++)
    {
        date_ptr[i].year = value[i].getYear ();
        date_ptr[i].month = value[i].getMonth ();
        date_ptr[i].day = value[i].getDay ();
        date_ptr[i].timezone_hours = value[i].getTimezoneHours ();
        date_ptr[i].timezone_minutes = value[i].getTimezoneMinutes ();
    }
    
    date_vect.values = date_ptr;
    date_vect.size = len;
    
    cvalue->type = WaveNs::WAVE_CVALUE_DATE_VECTOR;
    cvalue->value.date_vector_value = date_vect;
}


static inline void wave_cvalue_set_date_time (WaveCValue *cvalue, const WaveNs::DateTime value)
{
    wave_date_time date_time_value;
    
    date_time_value.year = value.getYear ();
    date_time_value.month = value.getMonth ();
    date_time_value.day = value.getDay ();
    date_time_value.hour = value.getHour ();
    date_time_value.min = value.getMin ();
    date_time_value.sec = value.getSec ();
    date_time_value.micro = value.getMicro ();
    date_time_value.timezone_hours = value.getTimezoneHours ();
    date_time_value.timezone_minutes = value.getTimezoneMinutes ();
    
    cvalue->type = WaveNs::WAVE_CVALUE_DATE_TIME;
    cvalue->value.datetime_value = date_time_value;
}


static inline void wave_cvalue_set_date_time_vector (WaveCValue *cvalue, const vector<WaveNs::DateTime> value)
{
    wave_date_time_vector date_time_vect;

    uint32_t i = 0;
    uint32_t len = value.size ();
    
    wave_date_time* date_time_ptr = new wave_date_time[len];
    WaveNs::prismAssert (NULL != date_time_ptr, __FILE__, __LINE__);
	
    for(i = 0; i < len; i++)
    {
        date_time_ptr[i].year = value[i].getYear ();
        date_time_ptr[i].month = value[i].getMonth ();
        date_time_ptr[i].day = value[i].getDay ();
        date_time_ptr[i].hour = value[i].getHour ();
        date_time_ptr[i].min = value[i].getMin ();
        date_time_ptr[i].sec = value[i].getSec ();
        date_time_ptr[i].micro = value[i].getMicro ();
        date_time_ptr[i].timezone_hours = value[i].getTimezoneHours ();
        date_time_ptr[i].timezone_minutes = value[i].getTimezoneMinutes ();
    }
	
    date_time_vect.datetime_values = date_time_ptr;
    date_time_vect.size = len;
	
    cvalue->type = WaveNs::WAVE_CVALUE_DATE_TIME_VECTOR;
    cvalue->value.datetime_vector_value = date_time_vect;
}


static inline void wave_cvalue_set_decimal_64 (WaveCValue *cvalue, const WaveNs::Decimal64 value)
{
    wave_decimal_64 decimal;
    
    decimal.decimal_value = value.getVal ();
    decimal.fraction_digits = value.getFractionDigits ();
    
    cvalue->type = WaveNs::WAVE_CVALUE_DECIMAL_64;
    cvalue->value.decimal64_value = decimal;
}


static inline void wave_cvalue_set_decimal_64_vector (WaveCValue *cvalue, const vector<WaveNs::Decimal64> value)
{
    wave_decimal_64_vector decimal_vect;
	
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    wave_decimal_64* decimal_64_ptr = new wave_decimal_64[len];
    WaveNs::prismAssert (NULL != decimal_64_ptr, __FILE__, __LINE__);
	
    for(i = 0; i < len; i++)
    {
        decimal_64_ptr[i].decimal_value = value[i].getVal ();
        decimal_64_ptr[i].fraction_digits = value[i].getFractionDigits ();
    }
	
    decimal_vect.decimal_64_values = decimal_64_ptr;
    decimal_vect.size = len;
	
    cvalue->type = WaveNs::WAVE_CVALUE_DECIMAL_64_VECTOR;
    cvalue->value.decimal64_vector_value= decimal_vect;
}

static inline void wave_cvalue_set_host (WaveCValue *cvalue, const WaveNs::HostUC value)
{
    wave_host host_value;
    
    host_value.host_type = value.getHostType ();

    host_value.address = strdup ((value.getHostValue()).c_str());
    WaveNs::prismAssert (NULL != host_value.address, __FILE__, __LINE__);
    
    cvalue->type = WaveNs::WAVE_CVALUE_HOST;
    cvalue->value.host_value = host_value;
}

static inline void wave_cvalue_set_ipv4address_vector (WaveCValue *cvalue, const vector<WaveNs::IpV4Address> value) 
{
    wave_ipaddress_vector ip_vect;
    
    uint32_t i ;
    uint32_t len = value.size ();
  	
    wave_ipaddress* ip_ptr = new wave_ipaddress[len];
    WaveNs::prismAssert (NULL != ip_ptr, __FILE__, __LINE__);
    
    for(i=0;i<len;i++)
    {
    	ip_ptr[i].ip_type = AF_INET ;  
    	
        ip_ptr[i].ip_address = strdup ((value[i].toString()).c_str()) ;
        WaveNs::prismAssert (NULL != ip_ptr[i].ip_address, __FILE__, __LINE__);
    }
    
    ip_vect.size=len;
    ip_vect.values=ip_ptr;
    
    cvalue->type = WaveNs::WAVE_CVALUE_IPADDRESS_VECTOR;
    cvalue->value.ipaddress_vector_value = ip_vect;
}

static inline void wave_cvalue_set_ipv6address_vector (WaveCValue *cvalue, const vector<WaveNs::IpV6Address> value) 
{
    wave_ipaddress_vector ip_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
  	
    wave_ipaddress* ip_ptr = new wave_ipaddress[len];
    WaveNs::prismAssert (NULL != ip_ptr, __FILE__, __LINE__);
    
    for(i=0;i<len;i++)
    {
    	ip_ptr[i].ip_type = AF_INET6 ;  
    	
        ip_ptr[i].ip_address = strdup ((value[i].toString()).c_str()) ;
        WaveNs::prismAssert (NULL != ip_ptr[i].ip_address, __FILE__, __LINE__);
    }

    ip_vect.size=len;
    ip_vect.values=ip_ptr;
    
    cvalue->type = WaveNs::WAVE_CVALUE_IPADDRESS_VECTOR;
    cvalue->value.ipaddress_vector_value = ip_vect;
}

static inline void wave_cvalue_set_ipvxaddress_vector (WaveCValue *cvalue, const vector<WaveNs::IpVxAddress> value)
{  
    wave_ipaddress_vector ip_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    wave_ipaddress* ip_ptr = new wave_ipaddress[len];
    WaveNs::prismAssert (NULL != ip_ptr, __FILE__, __LINE__);

    for(i=0;i<len;i++)
    {
        ip_ptr[i].ip_type = value[i].getIpType () ;
        
        ip_ptr[i].ip_address = strdup ((value[i].toString()).c_str()) ;
        WaveNs::prismAssert (NULL != ip_ptr[i].ip_address, __FILE__, __LINE__);
    }

    ip_vect.size=len;
    ip_vect.values=ip_ptr;
    
    cvalue->type = WaveNs::WAVE_CVALUE_IPADDRESS_VECTOR;
    cvalue->value.ipaddress_vector_value = ip_vect;
}


static inline void wave_cvalue_set_large_object (WaveCValue *cvalue, const WaveNs::LargeObject  value)
{
    wave_large_object large_obj;
    
    large_obj.number_of_bytes = value.getNumberOfBytes();
    
    large_obj.byte_blocks = strdup ((value.toString()).c_str()) ; 
    WaveNs::prismAssert (NULL != large_obj.byte_blocks, __FILE__, __LINE__);
    
    cvalue->type = WaveNs::WAVE_CVALUE_LARGEOBJECT;
    cvalue->value.large_object_value = large_obj;
}



static inline void wave_cvalue_set_snmp_objectid (WaveCValue *cvalue, const WaveNs::SnmpObjectId  value)
{
    wave_snmp_objectid snmp_oid;
    
    value.getOid (snmp_oid.oid);
    snmp_oid.length = value.getLength ();
    
    cvalue->type = WaveNs::WAVE_CVALUE_SNMP_OBJECT_ID;
    cvalue->value.snmp_object_id_value = snmp_oid;
}


static inline void wave_cvalue_set_time (WaveCValue *cvalue, const WaveNs::Time  value)
{
    wave_time time_value;
	
    time_value.hour = value.getHour ();
    time_value.min = value.getMin ();
    time_value.sec = value.getSec ();
    time_value.micro = value.getMicro ();
    time_value.timezone = value.getTimezoneHours ();
    time_value.timezone_minutes =value.getTimezoneMinutes (); 	
    
    cvalue->type = WaveNs::WAVE_CVALUE_TIME;
    cvalue->value.time_value = time_value;
}


static inline void wave_cvalue_set_time_vector (WaveCValue *cvalue, const vector<WaveNs::Time> value)
{
    wave_time_vector time_vect;
	
    uint32_t i = 0;
    uint32_t len = value.size ();

    wave_time* time_ptr = new wave_time[len];
    WaveNs::prismAssert (NULL != time_ptr, __FILE__, __LINE__);
	
    for(i = 0; i < len; i++)
    {
        time_ptr[i].hour = value[i].getHour ();
        time_ptr[i].min = value[i].getMin ();
        time_ptr[i].sec = value[i].getSec ();
        time_ptr[i].micro = value[i].getMicro ();
        time_ptr[i].timezone = value[i].getTimezoneHours ();
        time_ptr[i].timezone_minutes =value[i].getTimezoneMinutes (); 	
    }
	
    time_vect.values = time_ptr;
    time_vect.size = len;
    
    cvalue->type = WaveNs::WAVE_CVALUE_TIME_VECTOR;
    cvalue->value.time_vector_value= time_vect;
}


static inline void wave_cvalue_set_world_wide_name (WaveCValue *cvalue, const string value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME;

    cvalue->value.String = strdup (value.c_str());
    WaveNs::prismAssert (NULL != cvalue->value.String, __FILE__, __LINE__);
}

static inline void wave_cvalue_set_world_wide_name_vector (WaveCValue *cvalue, const vector<string> value)
{
    wave_string_vector str_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    char **strvalues = new char*[len];
    WaveNs::prismAssert (NULL != strvalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        strvalues[i]= strdup (value[i].c_str());
        WaveNs::prismAssert (NULL != strvalues[i], __FILE__, __LINE__);
    }

    str_vect.size = len;
    str_vect.values = strvalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME_VECTOR;
    cvalue->value.string_vector_value = str_vect;
}


static inline void wave_cvalue_set_macaddress (WaveCValue *cvalue, const string value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_MAC_ADDRESS;
    
    cvalue->value.String = strdup (value.c_str());
    WaveNs::prismAssert (NULL != cvalue->value.String, __FILE__, __LINE__);
}

static inline void wave_cvalue_set_macaddress_vector (WaveCValue *cvalue, const vector<string> value)
{
    wave_string_vector str_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    char **strvalues = new char*[len];
    WaveNs::prismAssert (NULL != strvalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        strvalues[i]= strdup (value[i].c_str());
        WaveNs::prismAssert (NULL != strvalues[i], __FILE__, __LINE__);
    }

    str_vect.size = len;
    str_vect.values = strvalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_MAC_ADDRESS_VECTOR;
    cvalue->value.string_vector_value = str_vect;
}


static inline void wave_cvalue_set_macaddress2 (WaveCValue *cvalue, const string value)
{
    cvalue->type = WaveNs::WAVE_CVALUE_MAC_ADDRESS2;
    
    cvalue->value.String = strdup (value.c_str());
    WaveNs::prismAssert (NULL != cvalue->value.String, __FILE__, __LINE__);
}


static inline void wave_cvalue_set_macaddress2_vector (WaveCValue *cvalue, const vector<string> value)
{
    wave_string_vector str_vect;
    
    uint32_t i;
    uint32_t len = value.size ();
    
    char **strvalues = new char*[len];
    WaveNs::prismAssert (NULL != strvalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        strvalues[i]= strdup (value[i].c_str());
    }

    str_vect.size = len;
    str_vect.values = strvalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_MAC_ADDRESS2_VECTOR;
    cvalue->value.string_vector_value = str_vect;
}


static inline void wave_cvalue_set_mac_range (WaveCValue *cvalue, const vector<string> value)
{
    wave_string_vector str_vect;
    
    uint32_t i = 0;
    uint32_t len = value.size ();
    
    char **strvalues = new char*[len];
    WaveNs::prismAssert (NULL != strvalues, __FILE__, __LINE__);
    
    for(i = 0; i < len; i++)
    {
        strvalues[i]= strdup (value[i].c_str());
        WaveNs::prismAssert (NULL != strvalues[i], __FILE__, __LINE__);
    }

    str_vect.size = len;
    str_vect.values = strvalues;
    
    cvalue->type = WaveNs::WAVE_CVALUE_MAC_RANGE;
    cvalue->value.string_vector_value = str_vect;
}


static inline int str2_ipv4_prefix (const char *str, struct wave_ipvx_prefix *p)
{
    int status = 0;
    int plen;
    const char *pnt;
    char *cp;

    // Find slash inside string. 
    pnt = strchr (str, '/');

    // String doesn't contail slash.  
    if (NULL == pnt)
    { 
        // Convert string to Ipv4 address. A nonzero return means successfull conversion and zero failed conversion. 
        status = inet_aton (str, &p->ip.ipaddressv4);
        if (0 == status)  
        {
            return 0;  // failed to do conversion of string to ipv4 format
        }
        // If address doesn't contain slash we assume it host address. 
        p->family = AF_INET;
        p->prefixlen = IPV4_MAX_BITLEN;

        return status;
    }
    else
    {
        cp = (char * )malloc ((pnt - str) + 1);
       
        if (NULL == cp)
        {
            return 0;    //malloc failure 
        }

        strncpy (cp, str, pnt - str);
        *(cp + (pnt - str)) = '\0';
        // Convert string to Ipv4 address. A nonzero return means successfull conversion and zero failed conversion.
        status = inet_aton (cp, &p->ip.ipaddressv4); 
        free (cp);

        // Get prefix length. 
        plen = (u_char) atoi (++pnt);

        if (plen > IPV4_MAX_BITLEN)
        {
            return 0;         // Failure :: return 0 in case the length is more then 32 
        }    

        p->family = AF_INET;
        p->prefixlen = plen;
    }
    return status;
}

static inline int str2_ipv6_prefix (const char *str, struct wave_ipvx_prefix  *p)
{
    const char *pnt;
    char *cp;
    int status = 0;

    pnt = strchr (str, '/');

    // If string doesn't contain `/' treat it as host route. 
    if (NULL == pnt)
    {  
        // Convert string to Ipv6 address. A nonzero return means successfull conversion and zero failed conversion.
        status = inet_pton (AF_INET6, str, &p->ip.ipaddressv4);
        if (0 == status)
        {
            return 0; 
        }     
        p->prefixlen = IPV6_MAX_BITLEN;
    }
    else
    { 
        int plen;

        cp = (char *) malloc ((pnt - str) + 1);
        if (NULL == cp)
        {
            return 0;    //malloc failure 
        }

        strncpy (cp, str, pnt - str);
        *(cp + (pnt - str)) = '\0';
        // Convert string to Ipv6 address. A nonzero return means successfull conversion and zero failed conversion.
        status = inet_pton (AF_INET6, cp, &p->ip.ipaddressv6);
        free (cp);
        if (0 == status)
        {
            return status;
        }    
        plen = (u_char) atoi (++pnt);

        if (plen > IPV6_MAX_BITLEN)
        {
            return 0;         // Failure :: return 0 in case the length is more then 128
        }

        p->prefixlen = plen;
    }
    p->family = AF_INET6;

    return status;
}

/* Generic function for conversion string to struct ipvx_prefix. */ 
static inline int str2_ipvx_prefix (const char *str, struct wave_ipvx_prefix *p)
{     

    int status = 0;
    
    status = str2_ipv4_prefix (str, p);
    
    if (0 == status)
    {  
        status = str2_ipv6_prefix (str, p);
    }

    return status;
}

static inline void wave_cvalue_set_ipaddress (WaveCValue *cvalue, const WaveNs::IpAddress *value)
{
    wave_ipvx_prefix ip;
    int status;
    
    status = str2_ipvx_prefix ((value->toString()).c_str(), &ip);
   
    if (0 == status)
    {
        printf ("wave_cvalue_set_ipvaddress : Invalid IPaddress passed to wave_cvalue_set_ipvaddress ");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }
  
    cvalue->type = WaveNs::WAVE_CVALUE_IPADDRESS;
    cvalue->value.ipvx_prefix_value = ip;
}

static inline void wave_cvalue_set_ipaddress_networkmask (WaveCValue *cvalue, const WaveNs::IpAddressNetworkMask* value)
{
    wave_ipvx_prefix ip;

    int status = str2_ipvx_prefix ((value->toString()).c_str(), &ip);

    if (0 == status)
    {   
        printf (" wave_cvalue_set_ipaddress_networkmask : Invalid IPaddress passed to wave_cvalue_set_ipaddress_networkmask ");
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    cvalue->type = WaveNs::WAVE_CVALUE_IPADDRESS_MASK;
    cvalue->value.ipvx_prefix_value = ip;
}

#ifdef __cplusplus
}
#endif
#endif //WAVESETCVALUE_H

