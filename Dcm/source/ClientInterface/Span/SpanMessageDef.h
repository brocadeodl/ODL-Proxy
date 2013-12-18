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

#ifndef __SPAN_MESSAGE_DEF_H__
#define __SPAN_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"

#define SPAN_PLUGIN_LOCAL_VERSION   0
#define SPAN_PLUGIN_GLOBAL_VERSION  0

#define MAX_TENGIGA_PORT_NUM_LEN 15
#define MAX_DESCRIPTION_LENGTH 1024
#define MAX_DESCRIPTION_LEN 64
#define MAX_DIRECTION_LENGTH 5
#define SPAN_OUT_OF_RANGE_VALUE  0xFFFFFFFF
#define IFTYPE_SIZE 20

#define STATE_ENABLED 1
#define MAX_SESSIONS_SUPPORTED_CALLISTO 24
#define MAX_SESSIONS_SUPPORTED_CASTOR 64
#define MAX_SESSIONS_SUPPORTED_MERCURY 48
#define DESTINATION_TYPE_RSPAN 3

#define SPAN_DIRECTION_TX 0
#define SPAN_DIRECTION_RX 1
#define SPAN_DIRECTION_BOTH 2

typedef enum {
	/* Span configure */
	SPAN_MSG_SET,
	SPAN_MSG_NO_SET,
	SPAN_SHOW_ALL,
	SPAN_SHOW_SESSION,
	SPAN_NO_LINECARD
} span_op_codes;

typedef enum {
	SPAN_BUFFER
} span_buffer_num;

typedef enum {
	SPAN_DEL_MIRRORED_PORT,
	SPAN_ADD_MIRRORED_PORT
}span_nsm_opcode;

typedef enum {
	/*Span message types */
	SPAN_LOCAL_MSG_DESC,
	SPAN_LOCAL_MSG_CONFIG,
	SPAN_LOCAL_MSG_SESSION,
	SPAN_LOCAL_MSG_SHOW,
	SPAN_MSG_SHOW_PATH,
	SPAN_SFP_BREAKOUT_CLEAR_LOCAL_MSG_CONFIG
} span_message_types;

typedef struct span_config_msg_t {
     int    m_session_id;
	 char   m_source_ifName[MAX_TENGIGA_PORT_NUM_LEN];
	 char   m_dest_ifName[MAX_TENGIGA_PORT_NUM_LEN];
	 int    m_opCode;
	 int    m_messageType;
     int 	m_direction;
	 int 	m_dest_ethernet;
	 int	m_src_ethernet;
	 char 	m_description[MAX_DESCRIPTION_LENGTH];
	 int 	m_vlan_id;
	 int    m_trill_sa;
	 int    m_trill_da;
} span_config_msg_t;

typedef struct span_config_error_t {
	int    m_state;
	int    *error_result;
}span_config_error_t;

typedef struct span_show_config_msg_ {
	unsigned int m_session_id;
	unsigned int m_messageType;
	unsigned int cmdCode;
}span_show_config_msg_;

typedef enum {
	SPAN_SOURCE_NODE,
	SPAN_DESTINATION_NODE,
	SPAN_INTERMEDIETE_NODE,
	SPAN_SOURCE_DESTINATION_NODE
}span_nsm_nodetype;

typedef struct span_nsm_show_msg_t {
	int    m_session_id;
	char   m_source_ifName[MAX_TENGIGA_PORT_NUM_LEN];
	char   m_dest_ifName[MAX_TENGIGA_PORT_NUM_LEN];
	char   m_direction[MAX_DIRECTION_LENGTH];
    char   m_description[MAX_DESCRIPTION_LENGTH];
	int	   m_src_state;
	int    m_dst_state;
	int	   m_vlan_id;
    int    m_state_source_port;
    int    m_state_dest_port;
	int    nbr_rbridge_id;
}span_nsm_show_msg_t;

typedef struct span_nsm_show_msg_info_ {
	unsigned int num_records;
	unsigned int record_type;
	union {
		span_nsm_show_msg_t session_id[1];
	} records;
} span_nsm_show_msg_info_t;

typedef union SpanBufferNum_ {
	struct {
		u_int16_t   mappedId;
		u_int16_t   tag;
	} buff;
	u_int32_t   num;
} SpanBufferNum_t;

typedef enum span_dcm_error_code_ {
    /* starting enum value from 100 to avoid any conflict */

    /* span event */
        DCM_SPAN_PORT_MIRROR_NOT_DELETE = 100,
        DCM_SPAN_INTERFACE_DOES_NOT_EXIST,
        DCM_SPAN_SOURCE_DEST_PORT_SAME,
        DCM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG,
        DCM_SPAN_INCORRECT_DIRECTION,
        DCM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION,
        DCM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED,
        DCM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED,
        DCM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR,
        DCM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION,
        DCM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT,
        DCM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION,
        DCM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED,
        DCM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT,
        DCM_SPAN_SESSION_DOESNOT_EXIST,
        DCM_SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR,
		DCM_SPAN_DEST_PORT_IN_L2,
		DCM_SPAN_DEST_PORT_IN_L3,
		DCM_SPAN_DEST_PORT_IN_MTU,
		DCM_SPAN_DEST_PORT_IN_LLDP,
		DCM_SPAN_DEST_PORT_IN_DOT1X,
		DCM_SPAN_DEST_PORT_IN_RMON,
		DCM_SPAN_DEST_PORT_IN_SFLOW,
		DCM_SPAN_DEST_PORT_IN_QoS,
		DCM_SPAN_DEST_PORT_IN_PIM,
		DCM_SPAN_DEST_PORT_IN_OSPF,
		DCM_SPAN_DEST_PORT_IN_IGMP,
		DCM_SPAN_DEST_PORT_IN_ACL,
		DCM_SPAN_DEST_PORT_IN_FCOE,
		DCM_SPAN_DEST_PORT_IN_LACP,
		DCM_NSM_SLOT_NOT_UP
} span_dcm_error_code_t;


static inline u_int16_t SpanGetBufferTag (u_int32_t  buffNum)
{
	return (((SpanBufferNum_t*) &buffNum)->buff.tag);
}

static inline u_int16_t SpanGetBufferMappedId (u_int32_t buffNum)
{
	return (((SpanBufferNum_t*) &buffNum)->buff.mappedId);
}

static inline u_int32_t SpanGetBufferNum (u_int32_t buffNum)
{
	return (((SpanBufferNum_t*) &buffNum)->num);
}

#endif	/* __SPAN_MESSAGE_DEF_H__ */
