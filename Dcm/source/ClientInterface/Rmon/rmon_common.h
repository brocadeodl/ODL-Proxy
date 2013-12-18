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

#ifndef _RMON_COMMON_H
#define _RMON_COMMON_H

#include <stdio.h>
#include "ClientInterface/DceWaveClientMessage.h"

#define RMON_MAX_WORD_LEN 			256
//6+2+1
#define RMON_LAST_TIME_SENT_WORD_LEN 9
#define DEFAULT_COMMUNITY_NAME 		"__default_community"
#define DEFAULT_DESCRIPTION 		"__default_description"
#define DEFAULT_OWNER 				"RMON_SNMP"
#define DCM_IFNAME_LENGTH 			50
#define DEFAULT_BUCKETS				50
#define DEFAULT_INTERVAL			1800
#define RMON_MAX_ALARM_OID_LEN		40	
#define DEFAULT_FALLING_EVENT_ID	0 
#define DEFAULT_FALLING_EVENT_THRESHOLD	0 
#define DCM_RMON_EVENT_NONE			1
#define DCM_RMON_EVENT_LOG			2
#define DCM_RMON_EVENT_TRAP			3
#define DCM_RMON_EVENT_LOG_TRAP		4
#define DCM_BUFFER_SIZE				30
#define RMON_MAX_LOG_ENTRIES 		32
#define DCM_RMON_VALID_STATUS                    1
#define RMON_PAGE_SIZE				50
#define UNKNOWN_COMMAND_CODE		9999


typedef struct rmon_event_message_ {
	unsigned int eventIndex;
    unsigned char  log;
    char   communityName[RMON_MAX_WORD_LEN];
    char   description[RMON_MAX_WORD_LEN];
    char   owner[RMON_MAX_WORD_LEN];
} rmon_event_message_t;

typedef struct rmon_alarm_message_ {
	unsigned int alarm_index;
	unsigned int interval;
	unsigned int sample_type;
	unsigned int rising_event;
	unsigned int rising_threshold;
	unsigned int falling_event;
	unsigned int falling_threshold;
    char   owner[RMON_MAX_WORD_LEN + 1];
	char   snmp_oid[RMON_MAX_ALARM_OID_LEN + 1];
} rmon_alarm_message_t;

typedef struct rmon_stats_message_ {
  	char ifname[DCM_IFNAME_LENGTH + 1];
	unsigned int stats_index;
    char   owner[RMON_MAX_WORD_LEN + 1];
} rmon_stats_message_t;

typedef struct rmon_history_message_ {
  	char ifname[DCM_IFNAME_LENGTH + 1];
	unsigned int history_index;
	unsigned int buckets;
	unsigned int interval;
    char   owner[RMON_MAX_WORD_LEN + 1];
} rmon_history_message_t;

typedef struct RmonClearInputMsg_ {
	int statsId;
	int cmdCode;
} RmonClearInputMsg_t;

typedef struct rmon_show_stats_input_msg_ {
	int stats_index;
	unsigned char show_all;
} rmon_show_stats_input_msg_t;

typedef struct rmon_show_history_input_msg_ {
	int history_index;
	unsigned char show_all;
} rmon_show_history_input_msg_t;

typedef struct rmon_show_event_input_msg_ {
	int event_index;
	unsigned char show_all;
} rmon_show_event_input_msg_t;

typedef struct rmon_show_alarm_input_msg_ {
	int alarm_index;
	unsigned char show_all;
} rmon_show_alarm_input_msg_t;

typedef struct rmon_show_input_pagination_attr_ {
	int startIndex;
	int endIndex;
	int startSampleIndex;
	int endSampleIndex;
} rmon_show_input_pagination_t;

typedef struct rmon_show_input_msg_ {
  	int command_code;
	int index;
	enum RQST_TYPE { get, get_next } requestType;
	unsigned char show_all;
	unsigned char show_brief;
	rmon_show_input_pagination_t rmonPaginationAttr;
} rmon_show_input_msg_t;

typedef struct rmon_show_statistics_message_ {
	int stats_index;
	unsigned long long interface_index;
	char interface_name[DCM_IFNAME_LENGTH];
	char packets[DCM_BUFFER_SIZE];
	char bytes[DCM_BUFFER_SIZE];
	char packets_dropped[DCM_BUFFER_SIZE];
	char multicasts[DCM_BUFFER_SIZE];
	char broadcasts[DCM_BUFFER_SIZE];
	char under_size[DCM_BUFFER_SIZE];
	char jabbers[DCM_BUFFER_SIZE];
	char crc[DCM_BUFFER_SIZE];
	char fragments[DCM_BUFFER_SIZE];
	char collisions[DCM_BUFFER_SIZE];
	char packets_64_bytes[DCM_BUFFER_SIZE];
	char packets_65_to_127_bytes[DCM_BUFFER_SIZE];
	char packets_128_to_255_bytes[DCM_BUFFER_SIZE];
	char packets_256_to_511_bytes[DCM_BUFFER_SIZE];
	char packets_512_to_1023_bytes[DCM_BUFFER_SIZE];
	char packets_1024_to_1518_bytes[DCM_BUFFER_SIZE];
	char packets_over_1518_bytes[DCM_BUFFER_SIZE];
	char   owner[RMON_MAX_WORD_LEN];
	int status;
} rmon_show_statistics_message_t;

typedef struct rmon_show_statistics_brief_message_ {
	int stats_index;
	unsigned long long interface_index;
	char interface_name[DCM_IFNAME_LENGTH];
} rmon_show_statistics_brief_message_t;

typedef struct rmon_show_history_message_ {
	int history_index;
	unsigned long long interface_index;
	char interface_name[DCM_IFNAME_LENGTH];
	int buckets_requested;
	int buckets_granted;
	int sampling_interval;
	char   owner[RMON_MAX_WORD_LEN];
} rmon_show_history_message_t;

typedef struct rmon_show_history_statistics_message_ {
	int history_index;
	int sample_index;
	char packets[DCM_BUFFER_SIZE];
	char octets[DCM_BUFFER_SIZE];
	char drop_events[DCM_BUFFER_SIZE];
	char broadcasts[DCM_BUFFER_SIZE];
	char multicasts[DCM_BUFFER_SIZE];
	char crc[DCM_BUFFER_SIZE];
	char undersize_packets[DCM_BUFFER_SIZE];
	char oversize_packets[DCM_BUFFER_SIZE];
	char fragments[DCM_BUFFER_SIZE];
	char jabbers[DCM_BUFFER_SIZE];
	char collisions[DCM_BUFFER_SIZE];
	char utilization[DCM_BUFFER_SIZE];
} rmon_show_history_statistics_message_t;

typedef struct rmon_log_table_
{
	u_int32_t logEventIndex;
	u_int32_t	logIndex;
	int32_t	logTime;
	char logDescription[RMON_MAX_WORD_LEN];
} rmon_log_table_t;

typedef struct rmon_show_logs_message_ 
{
	u_int32_t eventIndex;
	u_int8_t eventConf;
	u_int32_t eventType; 
	int32_t eventLastTimeSent;
	u_int32_t nextLogEntry;
	rmon_log_table_t logTable[RMON_MAX_LOG_ENTRIES];
	u_int32_t eventStatus;
	char     eventOwner [RMON_MAX_WORD_LEN];
	char     eventCommunity[RMON_MAX_WORD_LEN];
	char eventDescription[RMON_MAX_WORD_LEN];
} rmon_show_logs_message_t;

typedef struct rmon_show_alarms_message_ 
{
	int index;
	int status;
	char variable[RMON_MAX_WORD_LEN];
	unsigned int interval;
	int sample_type;
	unsigned int value;
	unsigned int rising_threshold;
	int rising_event;
	unsigned int falling_threshold;
	int falling_event;
	char  owner[RMON_MAX_WORD_LEN];


} rmon_show_alarms_message_t;

typedef struct rmon_show_events_message_
{
	unsigned int status;
	int index;
    char description[RMON_MAX_WORD_LEN];
	unsigned int type;
	char community[RMON_MAX_WORD_LEN];
	unsigned int  lastTimeSent;
	char owner[RMON_MAX_WORD_LEN];
}rmon_show_events_message_t;	

typedef struct rmon_show_output_pagination_attr_ {
	int hasMoreRecords;
	int nextIndex;
	int nextSampleIndex;
} rmon_show_output_pagination_t;

typedef struct rmon_get_stats_id_output_ {
	int statsId;
} rmon_get_stats_id_output_t;

typedef struct rmon_get_history_id_output_ {
	int historyId;
} rmon_get_history_id_output_t;

typedef struct rmon_get_event_id_output_ {
	int eventId;
} rmon_get_event_id_output_t;

typedef struct rmon_get_alarm_id_output_ {
	int alarmId;
} rmon_get_alarm_id_output_t;

typedef struct rmon_show_output_msg_ {
    unsigned int num_records;
    unsigned int record_type;
    rmon_show_output_pagination_t rmonPaginationAttr;
    union {
		rmon_show_statistics_message_t stats[1];
		rmon_show_history_message_t history[1];
		rmon_show_history_statistics_message_t history_statistics[1];
		rmon_show_logs_message_t logs[1];
		rmon_show_events_message_t events[1];
		rmon_show_alarms_message_t alarms[1];
		rmon_get_stats_id_output_t statsIds[1];
		rmon_get_history_id_output_t historyIds[1];
		rmon_get_event_id_output_t eventIds[1];
		rmon_get_alarm_id_output_t alarmIds[1];
    } records;
} rmon_show_output_msg_t;

typedef enum
{
	SHOW_RMON_HISTORY_CMD = 0,
	SHOW_RMON_HISTORY_STATISTICS_CMD,
	SHOW_RMON_STATISTICS_CMD,
	SHOW_RMON_LOGS_CMD,
	SHOW_RMON_EVENTS_CMD,
	SHOW_RMON_ALARMS_CMD,
	GET_ALL_RMON_STATS_ID_CMD,
	GET_ALL_RMON_HISTORY_ID_CMD,
	GET_ALL_RMON_EVENTS_ID_CMD,
	GET_ALL_RMON_ALARMS_ID_CMD
} show_command_types;

typedef enum
{
	SHOW_RMON_HISTORY = 0,
	SHOW_RMON_HISTORY_STATISTICS,
	SHOW_RMON_STATISTICS,
	SHOW_RMON_STATISTICS_BRIEF,
	SHOW_RMON_LOGS,
	SHOW_RMON_EVENTS,
	SHOW_RMON_EVENTS_BRIEF,
	SHOW_RMON_ALARMS,
	SHOW_RMON_ALARMS_BRIEF,
	GET_ALL_RMON_STATS_ID,
	GET_ALL_RMON_HISTORY_ID,
	GET_ALL_RMON_EVENTS_ID,
	GET_ALL_RMON_ALARMS_ID
} show_record_types;



typedef union rmon_mgmt_buffer_num_ {
    struct {
        u_int16_t   mapped_id;
        u_int16_t   tag;
    } buff;
    u_int32_t   num;
} rmon_mgmt_buffer_num_t;

static inline u_int16_t
rmon_get_buffer_tag (u_int32_t  buff_num)
{
      return (((rmon_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}


static inline u_int16_t
rmon_get_buffer_mapped_id (u_int32_t  buff_num)
{
      return (((rmon_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}


#endif 						// _RMON_COMMON_H
