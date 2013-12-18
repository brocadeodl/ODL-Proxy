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

#ifndef WAVECVALUE_H_
#define WAVECVALUE_H_


#include <stdio.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define IPV4_MAX_BITLEN    32
#define IPV6_MAX_BITLEN    128

struct wave_si32_vector {
    int32_t      *values;
    uint32_t      size;
};

struct wave_ui32_vector {
    uint32_t     *values;
    uint32_t      size;
};

struct wave_si64_vector {
    int64_t      *values;
    uint32_t      size;
};

struct wave_ui64_vector {
    uint64_t     *values;
    uint32_t      size;
};

struct wave_bitmap {
    uint64_t     *bit_blocks;
    uint32_t      number_of_bitblocks;
    uint32_t      number_of_bits;
};

struct wave_date {
    int16_t       year;
    uint8_t       month;
    uint8_t       day;
    int8_t        timezone_hours;
    int8_t        timezone_minutes;
};

struct wave_date_vector{
    struct wave_date  *values;
    uint32_t      size;
};

struct wave_date_time {
    int16_t       year;
    uint8_t       month;
    uint8_t       day;
    uint8_t       hour;
    uint8_t       min;
    uint8_t       sec;
    uint32_t      micro;
    int8_t        timezone_hours;
    int8_t        timezone_minutes;
};

struct wave_date_time_vector {
    struct wave_date_time    *datetime_values;
    uint32_t                  size;
};

struct wave_decimal_64 {
    int64_t       decimal_value;
    uint8_t       fraction_digits;
};

struct wave_decimal_64_vector {
    struct wave_decimal_64   *decimal_64_values;
    uint32_t                  size;
};

struct wave_host{
    char         *address;       //address can be one of these things 1)IpV4Address 2)IpV6Address 3)DomainValue
    uint32_t      host_type;
};


struct wave_ipaddress_networkmask {
    char         *ip_address;
    uint32_t      subnet_mask;
};

struct wave_ipaddress {
    char         *ip_address;
    uint32_t      ip_type;
};

// IPv4/v6 prefix structure.                                                                   
struct wave_ipvx_prefix 
{                                                                                             
    u_char family; 
    u_char prefixlen;
    union
    {
        struct in_addr  ipaddressv4; 
        struct in6_addr ipaddressv6;
    }ip;    
}; 

struct wave_ipaddress_vector{
    struct wave_ipaddress    *values;
    uint32_t             size;
};

struct wave_large_object {
    char         *byte_blocks;
    uint32_t      number_of_bytes;
};

struct wave_snmp_objectid {
    uint32_t      oid[128];
    uint32_t      length;
};

struct wave_string_vector{
    char        **values;
    uint32_t      size;
};

struct wave_time{
    uint8_t       hour;
    uint8_t       min;
    uint8_t       sec;
    uint32_t      micro;
    int8_t        timezone;
    int8_t        timezone_minutes;
};

struct wave_time_vector {
    struct wave_time  *values;
    uint32_t           size;
};

typedef struct
{
    uint32_t type;
    union
    {
        uint32_t                     ui32;          //For Bool, BoolUC, EnumUC, Enum,ResourceId, UI32, UI32UC
        int32_t                      si32;
        int64_t                      si64;          //used for SI64
        uint64_t                     ui64;          //used for UI64,
        uint16_t                     ui16;
        int16_t                      si16;
        uint8_t                      ui8;
        int8_t                       si8;
        char                         charvalue;
        char                        *String;        //String, Union, MACRange, UI32Range, StringUC, IpV4Address, IpV4AddressUC, IpV6Address, IpV6AddressUC
                                                    //WorldWideName, MacAddress, MacAddress2
        struct wave_string_vector         string_vector_value; //StringVector and StringVectorUC, WorldWideNameVector, MacAddressvector, MacAddress2Vector

        struct wave_si32_vector           si32_vector_value;
        struct wave_ui32_vector           ui32_vector_value; // used for ResourceIdVector and UI32Vector, UI32VectorUC, BoolVector
        struct wave_si64_vector           si64_vector_value;
        struct wave_ui64_vector           ui64_vector_value;

        struct wave_bitmap                bitmap_value;
        struct wave_date                  date_value;
        struct wave_date_vector           date_vector_value;
        struct wave_date_time             datetime_value;
        struct wave_date_time_vector      datetime_vector_value;
        struct wave_decimal_64            decimal64_value;
        struct wave_decimal_64_vector     decimal64_vector_value;
        struct wave_host                  host_value;
        struct wave_ipaddress_vector      ipaddress_vector_value;
        struct wave_large_object          large_object_value;
        struct wave_snmp_objectid         snmp_object_id_value;
        struct wave_time                  time_value;
        struct wave_time_vector           time_vector_value;
        struct wave_ipvx_prefix           ipvx_prefix_value; //used for all ip address and network mask
    } value;

}WaveCValue;

static inline int32_t wave_cvalue_get_bool (WaveCValue *cvalue)
{
    return (cvalue->value.ui32);
}


static inline int32_t wave_cvalue_get_ui32 (WaveCValue *cvalue)
{
    return (cvalue->value.ui32);
}


static inline int32_t wave_cvalue_get_si32 (WaveCValue *cvalue)
{
    return (cvalue->value.si32);
}


static inline int64_t wave_cvalue_get_si64 (WaveCValue *cvalue)
{
    return (cvalue->value.si64);
}

static inline int64_t wave_cvalue_get_ui64 (WaveCValue *cvalue)
{
    return (cvalue->value.ui64);
}

static inline int16_t wave_cvalue_get_ui16 (WaveCValue *cvalue)
{
    return (cvalue->value.ui16);
}

static inline int16_t wave_cvalue_get_si16 (WaveCValue *cvalue)
{
    return (cvalue->value.si16);
}


static inline int8_t wave_cvalue_get_ui8 (WaveCValue *cvalue)
{
    return (cvalue->value.ui8);
}


static inline int8_t wave_cvalue_get_si8 (WaveCValue *cvalue)
{
    return (cvalue->value.si8);
}


static inline char wave_cvalue_get_char (WaveCValue *cvalue)
{
    return (cvalue->value.charvalue);
}


static inline char* wave_cvalue_get_string (WaveCValue *cvalue)
{
    return (cvalue->value.String);
}

static inline struct wave_string_vector wave_cvalue_get_string_vector (WaveCValue *cvalue)
{
    return (cvalue->value.string_vector_value);
}

static inline struct wave_si32_vector wave_cvalue_get_si32_vector (WaveCValue *cvalue)
{
    return (cvalue->value.si32_vector_value);
}

static inline struct wave_ui32_vector wave_cvalue_get_bool_vector (WaveCValue *cvalue)
{
    return (cvalue->value.ui32_vector_value);
}

static inline struct wave_ui32_vector wave_cvalue_get_ui32_vector (WaveCValue *cvalue)
{
    return (cvalue->value.ui32_vector_value);
}

static inline struct wave_si64_vector wave_cvalue_get_si64_vector (WaveCValue *cvalue)
{
    return (cvalue->value.si64_vector_value);
}

static inline struct wave_ui64_vector wave_cvalue_get_ui64_vector (WaveCValue *cvalue)
{
    return (cvalue->value.ui64_vector_value);
}


static inline struct wave_bitmap wave_cvalue_get_bitmap (WaveCValue *cvalue)
{
    return (cvalue->value.bitmap_value);
}

static inline struct wave_date wave_cvalue_get_date(WaveCValue *cvalue)
{
    return (cvalue->value.date_value);
}

static inline struct wave_date_vector wave_cvalue_get_date_vector(WaveCValue *cvalue)
{
    return (cvalue->value.date_vector_value);
}

static inline struct wave_date_time wave_cvalue_get_date_time(WaveCValue *cvalue)
{
    return (cvalue->value.datetime_value);
}

static inline struct wave_date_time_vector wave_cvalue_get_date_time_vector (WaveCValue *cvalue)
{
    return (cvalue->value.datetime_vector_value);
}

static inline struct wave_decimal_64 wave_cvalue_get_decimal_64 (WaveCValue *cvalue)
{
    return (cvalue->value.decimal64_value);
}

static inline struct wave_decimal_64_vector wave_cvalue_get_decimal_64_vector (WaveCValue *cvalue)
{
    return (cvalue->value.decimal64_vector_value);
}

static inline struct wave_host wave_cvalue_get_host(WaveCValue *cvalue)
{
    return (cvalue->value.host_value);
}


static inline struct wave_ipaddress_vector wave_cvalue_get_ipaddress_vector (WaveCValue *cvalue)
{
    return (cvalue->value.ipaddress_vector_value);
}

static inline struct wave_large_object wave_cvalue_get_large_object (WaveCValue *cvalue)
{
    return (cvalue->value.large_object_value);
}

static inline struct wave_snmp_objectid wave_cvalue_get_snmp_objectid(WaveCValue *cvalue)
{
    return (cvalue->value.snmp_object_id_value);
}

static inline struct wave_time wave_cvalue_get_time (WaveCValue *cvalue)
{
    return (cvalue->value.time_value);
}


static inline struct wave_time_vector wave_cvalue_get_time_vector (WaveCValue *cvalue)
{
    return (cvalue->value.time_vector_value);
}

static inline char* wave_cvalue_get_world_wide_name(WaveCValue *cvalue)
{
    return (cvalue->value.String);
}

static inline struct wave_string_vector wave_cvalue_get_world_wide_name_vector (WaveCValue *cvalue)
{
    return (cvalue->value.string_vector_value);
}

static inline char* wave_cvalue_get_macaddress (WaveCValue *cvalue)
{
    return (cvalue->value.String);
}

static inline struct wave_string_vector wave_cvalue_get_macaddress_vector (WaveCValue *cvalue)
{
    return (cvalue->value.string_vector_value);
}


static inline char* wave_cvalue_get_macaddress2 (WaveCValue *cvalue)
{
    return (cvalue->value.String);
}

static inline struct wave_string_vector wave_cvalue_get_macaddress2_vector (WaveCValue *cvalue)
{
    return (cvalue->value.string_vector_value);
}

static inline struct wave_string_vector wave_cvalue_get_mac_range (WaveCValue *cvalue)
{
    return (cvalue->value.string_vector_value);
}

static inline struct wave_ipvx_prefix  wave_cvalue_get_ipaddress (WaveCValue *cvalue)
{
    return (cvalue->value.ipvx_prefix_value);
}


// Ussages    : wave_ipvx_prefix2str converts previx structure to string format
// Parameters : wave_ipvx_prefix structure pointer to be converted to string 
// Return     : NULL in case of failure. A character pointer to a dynamicaly allocated string is returned in case of success.
//              Caller should take care of deallocating this allocated memory else it will leak.
static inline char* wave_ipvx_prefix2str (struct wave_ipvx_prefix *p)
{
    char buf[BUFSIZ] = {0};
    
    char *str = (char *)calloc(IPV6_MAX_BITLEN,sizeof(char));  //sufficient for both ipv4 and ipv6 address as IPV6_MAX_BITLEN is 128
    if (str == NULL)
    {
        return NULL;                    // failed to allocate memory return NULL       
    }

    if (AF_INET == p->family)
    {
        inet_ntop (p->family, &p->ip.ipaddressv4, buf, BUFSIZ);

    }   
    else if (AF_INET6 == p->family)
    {
        inet_ntop (p->family, &p->ip.ipaddressv6, buf, BUFSIZ);
    }

    if (IPV6_MAX_BITLEN == p->prefixlen || IPV4_MAX_BITLEN == p->prefixlen)
    {
        snprintf (str,IPV6_MAX_BITLEN, "%s", buf);
    }
    else 
    {
        snprintf (str,IPV6_MAX_BITLEN, "%s/%d", buf, p->prefixlen);
    }    

    return str;     
}


#ifdef __cplusplus
}
#endif
#endif //WAVECVALUE_H

