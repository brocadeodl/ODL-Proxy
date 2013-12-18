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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Amitayu Das                                                 *
 *   File: lldp_dcm_def.h                                                   *
 *   Description:                                                           *
 *                                                                          *
 *      The common C structure definition for back-end to interface with    *
 *       DCMd                                                               *
 ***************************************************************************/

#ifndef ELDPDCMDEF_H
#define ELDPDCMDEF_H

#include <stdio.h>
//#include <unistd.h>
#include <stdint.h>

#define IS_CMDCODE_SET(cmdcode, flag) (cmdcode & flag)

/* ELD Global Command Codes */
#define        ELD_GLOBAL_CONF_VCS_RX_LIMIT         0x1<<1    
#define        ELD_GLOBAL_CONF_SHUTDOWN_TIME        0x1<<2    
#define        ELD_GLOBAL_CONF_ELD                  0x1<<3
#define        ELD_GLOBAL_CONF_NO_ELD               0x1<<4
#define        ELD_GLOBAL_CONF_HELLO_TIME           0x1<<5


// Intf Level cmds

#define        ELD_INTF_CONF_ELD_ENABLE             0x1<<10
#define        ELD_INTF_CONF_ELD_DISABLE            0x1<<11
#define        ELD_INTF_CONF_PRIORITY               0x1<<12
#define        ELD_INTF_PO_CONF_PRIORITY            0x1<<13
#define        ELD_INTF_PO_CONF_ELD_ENABLE          0x1<<14
#define        ELD_INTF_PO_CONF_ELD_DISABLE         0x1<<15

//ELD Global cmds default values
#define        ELD_DEFAULT_PDU_RX_LIMIT				1
#define        ELD_DEFAULT_HELLO_TIME				1000
#define        ELD_DEFAULT_SHUTDOWN_TIME			0
#define        ELD_DEFAULT_PRIORITY_VALUE			128

/* debug edge-loop-detection */
#define ELD_DEBUG_CLI_TKN_STR_LEN_MAX        32

#define ELD_DEBUG_CLI_TKN_STR_DEBUG          "debug"
#define ELD_DEBUG_CLI_TKN_STR_ELD            "edge-loop-detection"
#define ELD_DEBUG_CLI_TKN_STR_DUMP           "dump"
#define ELD_DEBUG_CLI_TKN_STR_PACKET         "packet"
#define ELD_DEBUG_CLI_TKN_STR_ALL            "all"
#define ELD_DEBUG_CLI_TKN_STR_DETAIL         "detail"
#define ELD_DEBUG_CLI_TKN_STR_BOTH           "both"
#define ELD_DEBUG_CLI_TKN_STR_TX             "tx"
#define ELD_DEBUG_CLI_TKN_STR_RX             "rx"
#define ELD_DEBUG_CLI_TKN_STR_NO             "no"
#define ELD_DEBUG_CLI_TKN_STR_TE_NAME        "tengigabitethernet"
#define ELD_DEBUG_CLI_TKN_INTF_DELIM         "/"

typedef enum {
    ELD_DEBUG_CMD_CODE_INVALID = 0,
    ELD_DEBUG_CMD_CODE_DUMP_ALL,
    ELD_DEBUG_CMD_CODE_DUMP_INTF,
    ELD_DEBUG_CMD_CODE_PACKET_ALL,
    ELD_DEBUG_CMD_CODE_PACKET_INTF,
    ELD_DEBUG_CMD_CODE_SHOW_STATUS
} eld_debug_cmd_t;

typedef enum eld_debug_buf_ {
    ELD_DEBUG_BUF_UNKNOWN = 0,
    ELD_DEBUG_BUF_DUMP_ALL,
    ELD_DEBUG_BUF_DUMP_INTF,
    ELD_DEBUG_BUF_PKT,
    ELD_DEBUG_BUF_SHOW_STATUS
} eld_debug_buf_t;

#define ELD_DEBUG_REQ_FLAG_RX             (1 << 0)
#define ELD_DEBUG_REQ_FLAG_TX             (1 << 1)
#define ELD_DEBUG_REQ_FLAG_DUMP           (1 << 2)
#define ELD_DEBUG_REQ_FLAG_DETAIL         (1 << 3)

/* debug edge-loop-detection */

#define MAX_STR  64
//#define uint32_t unsigned int
//#define uint16_t unsigned short

typedef struct _eld_global_conf {
    uint32_t cmd_code;
    uint32_t hello_interval;
    uint32_t vcs_rx_limit;
    uint32_t shutdown_time;
} eld_mgmt_global_conf_t;

typedef struct _eld_po_msg_s_ {
	char poId[MAX_STR];
	char poName[MAX_STR];
	unsigned int poIfIndex;
} eld_po_msg_t;

typedef struct _eld_intf_conf {
    uint32_t cmd_code;
    char if_name[MAX_STR];
	eld_po_msg_t po_info;
//    uint16_t vlan_id;
    uint16_t priority;
	uint32_t intf_type;		// TE or GI
    uint16_t vlan_num;  	// needed to support range
    uint32_t vid[1];		// needed to support range

}eld_mgmt_intf_conf_t;

typedef struct _eld_mgmt_conf_req{
    uint32_t cmd_code;

    union {
        eld_mgmt_global_conf_t     global_conf;
        eld_mgmt_intf_conf_t        intf_conf;
    } entry;
}eld_mgmt_conf_req_t;

typedef struct _eldshow_interface_inputmsg_ {
	uint32_t cmdcode;
	uint32_t rbridge_id;
    char if_name[MAX_STR];
    uint16_t vlan_id;
} eldshow_interface_inputmsg_t;

typedef struct _eldshow_stats_ {
	uint32_t 	total_rx_pkt;
	uint32_t 	total_tx_pkt;
} eldshow_stats_t;

typedef struct _eldshow_global_data_ {
	uint32_t 	pdu_rx_limit;
	uint32_t 	hello_time;
	uint32_t 	shutdown_time;
} eldshow_global_data_outputmsg_t;

typedef struct _vlan_level_details_ {
	uint32_t  	send_untagged;
	uint32_t    time_for_rxlimit;   // how long to wait for pdus to shutdown the port
}eld_vlan_level_details;

typedef struct eldshow_vlan_instance {
	uint32_t vlan_id;
	eldshow_stats_t	stats;
	eld_vlan_level_details  vlan_details;
} eldshow_vlan_instance_t;

typedef struct _interface_level_details_{
    uint32_t    vlag_master_id; // master rbridge for the vlag
	uint32_t    pvid_frame_type;    // TAGGED, UNTAGGED or BOTH
	uint32_t    brcd_agg_type;      // if this is a brocade trunk
	uint32_t  	port_type;          // access or trunk
    uint32_t    if_status;
    eldshow_stats_t    	total_stats;        // statistics   
} eld_interface_level_details;

typedef struct eldshow_eldif {
    char 	 	if_name[MAX_STR];
	uint32_t 	num_records;
	uint32_t 	priority;
	uint32_t 	shutdown_vlan;
	uint32_t 	time_duration;
	eld_interface_level_details interface_details;
	eldshow_vlan_instance_t eld_vlan_instance[1];
} eldshow_eld_if_t;

typedef struct _rbridge_level_details_ {
	unsigned char       eld_mac_address[6];
} eld_rbridge_level_details;

typedef struct eld_show_msg_ {
	uint32_t 	record_type;
	uint32_t 	num_records;
	uint32_t 	rbridge_id;
	uint16_t 	total_instances;// total num of eld instances
	uint32_t 	size;
	eld_rbridge_level_details rbridge_details;
	union {
		eldshow_global_data_outputmsg_t global_info; // show global 
		eldshow_eld_if_t eldshow_if[1];// eld if data
	} data;
}eld_show_msg_t;

typedef struct eld_clear_msg_ {
	uint32_t cmdcode;
        char if_name[MAX_STR];
        uint32_t if_type;
        
}eld_clear_msg_t;

typedef struct _elddebug_stats {
	uint32_t total_rx_pkt;
	uint32_t total_tx_pkt;
	uint32_t    disabled_count; // number ELD loop detected
} elddebug_stats_t;

typedef struct _elddebug_instance {
	uint32_t 			vlan_id;
    uint32_t            instance_if_index;           // i/f index    
    uint32_t            pdu_cnt_to_shutdown;// num pdu to shutdown i/f  
    uint32_t            seq_num;            // seq_num
    uint32_t            time_for_rxlimit;   // how long to wait for pdus to shutdown the
	elddebug_stats_t	stats;
} elddebug_instance_t;

typedef struct _elddebug_eldif {
    char 	 				if_name[MAX_STR];
	uint32_t 				num_records;
	uint32_t 				priority;
	uint32_t 				shutdown_vlan;
    uint32_t            	age_left;   	// disabled time in minute
    uint32_t             	pvid;          	// port vlan id
    uint32_t              	if_status;
    uint32_t             	disabled;   	// if port is disabled by ELD
    uint32_t             	vlag_master_id; // master rbridge for the vlag
	uint32_t    			pvid_frame_type;    // TAGGED, UNTAGGED or BOTH
	uint32_t    			dbgFlags;
	void 	 				*eld_if_address;
    void 					*ifp;
	uint32_t    			if_index;
    void             		*eld_instance_list; // eld i/f cfg for each vlan
    void 					*link_agg_if;       // eld_if of lag for this member port
    void      				*disabled_by_instance; // ELD node for which port shutdown
    elddebug_stats_t    	total_stats;        // statistics   
	elddebug_instance_t 	eld_instance[1];
} elddebug_eld_if_t;

typedef struct _eld_debug_resp_msg {
	uint32_t 			record_type;
	uint32_t 			num_records;
	uint32_t 			size;
	uint32_t 			sockfd;
	uint32_t 			rbridge_id;
	unsigned char       eld_mac_address[6];
	unsigned char       bridge_mac[6];
	void 				*eld_if_list;
	uint16_t            total_instances;// total num of eld instances
	uint32_t			reserved[8];	// Reserved for future usage
	elddebug_eld_if_t elddebug_if[1];
}eld_debug_resp_msg_t;

typedef struct _eld_debug_req_msg {
	uint32_t 	cmdcode;
	uint32_t 	rbridge_id;
    char 		if_name[MAX_STR];
    uint16_t 	vlan_id;
	uint32_t    dbgFlags;
}eld_debug_msg_t;

/* debug eld cmd code */

typedef enum 
	{
		ELD_DEBUG_UNKNOWN = 0,
		ELD_DEBUG_RBRIDGE = 1,
		ELD_DEBUG_INTERFACE = 2,
		ELD_INTERFACE_DEBUG = 3
	}EldDebugCmdCode;

/* clear eld cmd code */

typedef enum 
	{
		ELD_GLOBAL_CLEAR = 0,
                ELD_CLEAR_INTF_CONF

	}EldClearCmdCode;

/* show eld cmd code */
typedef enum
    {
        ELDSHOW_UNKNOWN = 0,
        ELDSHOW_SYSID,
		ELD_SHOW_INTF_VLAN, 	
		ELD_SHOW_RBRIDGE,
		ELD_SHOW_INTF,
		ELD_SHOW_GLOBAL,
		ELD_SHOW_ALL_RBRIDGE,
		ELD_SHOW_RBRIDGE_DETAIL
    }EldShowCmdCode;

typedef enum
    {
		ELD_BUF_UNKNOWN = 0,
		ELD_BUF_GLOBAL,
		ELD_BUF_INTERFACE,
		ELD_BUF_INTERFACE_VLAN, 
		ELD_BUF_RBRIDGE,
		ELD_BUF_RBRIDGE_DETAIL
	}EldMessageBufferId;

typedef union eld_mgmt_buffer_num_ {
    struct {
        unsigned short   mapped_id;
        unsigned short   tag;
    } buff;
    unsigned int num;
} eld_mgmt_buffer_num_t;


static inline unsigned short
eld_get_buffer_mapped_id (unsigned int  buff_num)
{
    return (((eld_mgmt_buffer_num_t*) &buff_num)->buff.mapped_id);
}

static inline unsigned short
 eld_get_buffer_tag (unsigned int  buff_num)
{
    return (((eld_mgmt_buffer_num_t*) &buff_num)->buff.tag);
}

#endif  //ELDPDCMDEF_H
