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

#ifndef __SFLOW_MESSAGE_DEF_H__
#define __SFLOW_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Ssm/qoscommon.h"

#define SFLOW_PLUGIN_LOCAL_VERSION   0
#define SFLOW_PLUGIN_GLOBAL_VERSION  0

#define SFLOW_DEFAULT_COLLECTOR 		""
#define SFLOW_DEFAULT_COLLECTOR_PORT         6343
#define SFLOW_DEFAULT_SAMPLE_RATE       32768
#define SFLOW_MIN_SAMPLE_RATE			2
#define SFLOW_MAX_SAMPLE_RATE       	16777215
#define SFLOW_DEFAULT_POLLING_INTERVAL  20
#define SFLOW_MIN_POLLING_INTERVAL		0
#define SFLOW_MAX_POLLING_INTERVAL		65535
#define SFLOW_DEFAULT_ENABLE            false
#define SFLOW_OUT_OF_RANGE_VALUE		0xFFFFFFFF
#define SFLOW_ZERO_COLLECTOR_IP         "0.0.0.0"
#define SFLOW_MULTICAST_BEGIN_IP        "224.0.0.0"
#define SFLOW_MULTICAST_END_IP          "239.255.255.255"
#define SFLOW_BROADCAST_ALL_IP          "255.255.255.255"
#define SFLOW_LOOPBACK_IP               "127.0.0.1"
#define MAX_LP  1
/* Make sure SFLOW_DCM_MAX_COLLECTOR_SERVERS = SFLOW_MAX_COLLECTOR_SERVERS */
// Changed from 5 to 1 to avoid sanity probs temporarily
#define SFLOW_DCM_MAX_COLLECTOR_SERVERS	5
#define SFLOW_DCM_MAX_PROFILE_COUNT     16
#define SFLOW_COLLECTOR_ADDRESSTYPE_IPV4	0
#define SFLOW_COLLECTOR_ADDRESSTYPE_IPV6	1
typedef enum {
	/* Sflow configure */
	SFLOW_MSG_ENABLE,
	SFLOW_MSG_COLLECTOR,
	SFLOW_MSG_POLLING_INTERVAL,
	SFLOW_MSG_SAMPLE_RATE,
	SFLOW_MSG_SET_ALL,

	SFLOW_MSG_NO_ENABLE,
	SFLOW_MSG_NO_COLLECTOR,
	SFLOW_MSG_NO_POLLING_INTERVAL,
	SFLOW_MSG_NO_SAMPLE_RATE,

	SFLOW_MSG_PROFILE_CREATE,
	SFLOW_MSG_PROFILE_DELETE,
	SFLOW_SHOW_SFLOWPROFILE,
	SFLOW_SHOW_SFLOWPROFILE_ALL
} sflow_cmd_code;

typedef enum
{
    SFLOW_CLEAR_STATS  = 1,
    SFLOW_CLEAR_ALL_STATS,
    SFLOW_CLEAR_INTERFACE_STATS 
} sflow_clear_cmd_code; 

typedef enum
{
    DEBUG_SFLOW = 1,
    DEBUG_NO_SFLOW,
    DEBUG_SHOW_SFLOW
} sflow_debug_cmd_type; 

typedef enum
{
    DEBUG_SFLOW_PACKET_INTF =1, 
    DEBUG_SFLOW_PACKET_ALL,
    DEBUG_SFLOW_COMMAND,
    DEBUG_SFLOW_DUMP
} sflow_debug_cmd_code; 

typedef struct sflow_global_config_msg_ {
	int	enable;
	char	collector[64];
	unsigned int collector_port;
	//collector_type = 0 => ip v4 address, collector_type = 1 => ip v6 address
	unsigned int collector_type;
	int	polling_interval;
	int	sample_rate;
	int	opcode;
} sflow_global_config_msg_t;

typedef struct sflow_show_input_msg_ {
         char if_name[MAX_STR+1];
         char interface[MAX_STR+1];
         unsigned int cmdCode;
} sflow_show_input_msg_t;
			    
typedef struct sflow_debug_input_msg_ {
    char if_name[MAX_STR+4];
    char interface[MAX_STR+4];
    unsigned int cmdType;
    unsigned int cmdCode;
    unsigned int subCode;
    unsigned int bpId;
}sflow_debug_input_msg_t;

typedef struct sflow_show_interface_msg_ {
         char ifLongName[MAX_STR+1];
         int sample_rate;
         int actual_sample_rate;
         int polling_interval;
         unsigned int sampled_packets;
         unsigned  counter_sampled_packets;
		 int backoffThreshold;
		 unsigned int port_based_services;
		 unsigned int flow_based_services;
		 uint8_t debug_flags;
#define SFLOW_DEBUG_PACKET	    (1 << 0)
} sflow_show_interface_msg_t;

typedef struct sflow_show_msg_ {
	unsigned int num_records;
	unsigned int record_type;
	int enabled;
	int sample_rate;
	int polling_interval;
	char collector[SFLOW_DCM_MAX_COLLECTOR_SERVERS][MAX_STR+1];
	int samples_sent[SFLOW_DCM_MAX_COLLECTOR_SERVERS];
	int slot_id;
    uint8_t debug_flags;
#define SFLOW_DEBUG_PACKET_ALL      (1 << 0)
#define SFLOW_DEBUG_COMMAND        (1 << 1)
	union {
		sflow_show_interface_msg_t interface[1];
	} records;
} sflow_show_msg_t;

typedef struct sflow_profile_config_msg_ {
	char profile_name[65];
	int	 sample_rate;
	int	 opcode;
} sflow_profile_config_msg_t;

typedef struct sflow_profile_show_input_msg_ {
         char profile_name[MAX_STR+1];
         unsigned int cmdCode;
} sflow_profile_show_input_msg_t;

typedef struct sflow_show_profile_msg_ {
         char profileName[MAX_STR+1];
         int sample_rate;
		 uint8_t debug_flags;
#define SFLOW_PROFILE_DEBUG_PACKET	    (1 << 0)
} sflow_show_profile_msg_t;

typedef struct sflow_profile_show_msg_ {
	unsigned int num_records;
	unsigned int record_type;
    uint8_t debug_flags;
#define SFLOW_PROFILE_DEBUG_PACKET_ALL      (1 << 0)
#define SFLOW_PROFILE_DEBUG_COMMAND        (1 << 1)
	union {
    	sflow_show_profile_msg_t profile[1];
	} records;
} sflow_profile_show_msg_t;


typedef union SflowBufferNum_ {
	struct {
		u_int8_t   mappedId;
		u_int8_t   slotId;
		u_int16_t   tag;
		} buff;
	u_int32_t   num;
} SflowBufferNum_t;

static inline u_int16_t SflowGetBufferTag (u_int32_t buffNum)
{
	return (((SflowBufferNum_t*) &buffNum)->buff.tag);
}

static inline u_int8_t SflowGetBufferMappedId (u_int32_t buffNum)
{
	return (((SflowBufferNum_t*) &buffNum)->buff.mappedId);
}

static inline u_int8_t SflowGetBufferSlotId (u_int32_t buffNum)
{
	return (((SflowBufferNum_t*) &buffNum)->buff.slotId);
}

static inline u_int32_t SflowGetBufferNum (u_int32_t buffNum)
{
	return (((SflowBufferNum_t*) &buffNum)->num);
}

#endif	/* __SFLOW_MESSAGE_DEF_H__ */
