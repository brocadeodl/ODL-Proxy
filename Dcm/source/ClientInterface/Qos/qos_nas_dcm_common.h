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

/*
 * Copyright (c) 2009 by Brocade Communications Systems, Inc.
 * @file qos_nas_dcm_common.h
 * @brief Header file containing definitions exposed to all
 * @date 04/29/2013
 *
 */
#ifndef _QOS_NAS_DCM_COMMON_H_
#define _QOS_NAS_DCM_COMMON_H_
#include "qosd_fb_public.h"
#include "DcmCStatus.h"

#define DEFAULT_NAS_COS 64 
#define DEFAULT_NAS_DSCP 64 
#define DEFAULT_NAS_VLAN_NUMBER  0 
#define DEFAULT_NAS_VRF_NAME "default-vrf" 
#define DEFAULT_NAS_VRF_ID		0 
#define DEFAULT_NAS_VRF_LEN  64 
#define AUTO_NAS_MAX_SERVER_IP_ALLOWED 64
#define AUTO_NAS_PGID_VALUE	"3"	
#define CEE_LAN_PGID_VALUE  "2"	
#define AUTO_NAS_PGID_WEIGHT 20
#define DEFAULT_AUTO_NAS_COS 2 
#define CEE_FABRIC_PRIORITY_COS 7 
/*
* Typedef, enumeration, structure definitions
*/

/**
 * Enumeration of opcodes used by front-end cli to backend module
 */
typedef enum
{
   QOS_NAS_DCMD_DISABLE = 100,
   QOS_NAS_DCMD_AUTO,
   QOS_NAS_DCMD_COS,
   QOS_NAS_DCMD_DSCP,
   QOS_NAS_DCMD_SERVER_IP, //For server-ip configuration, backend will always get this cmd_code
   QOS_NAS_DCMD_SERVER_IP_VLAN, // used only by DCMD
   QOS_NAS_DCMD_SERVER_IP_VRF, //used only by DCMD
   QOS_NAS_DCMD_SHOW_STATS,
   QOS_NAS_DCMD_CLEAR_STATS,
   QOS_NAS_DCMD_DEBUG
} qosNasDcmdCmdcode_t;

typedef enum
{
	QOS_NAS_DCMD_CONFIG = 100,
	QOS_NAS_DCMD_DE_CONFIG
} qosNasDcmdOpcode_t;

/**
 * Data type for NAS config requests
 */

/**
 * This data type is used to enable/disable NAS auto-qos, add/remove server ip to
 * server ip list.
 */

typedef struct qosNasDcmdConfigMsg_s
{
	qosNasDcmdOpcode_t opcode;
	qosNasDcmdCmdcode_t cmdcode;
	uint32_t cos;
	uint32_t dscp;
	uint32_t	 ip_addr;
	uint32_t	 mask;
	uint32_t vlan_number;
	char vrf_name[DEFAULT_NAS_VRF_LEN+1];

} qosNasDcmdConfigMsg_t;


typedef struct qosNasDcmdServerIfInfo_s
{
	bool_t 				is_vlan_present;
	uint32_t			vlan;
	char				vrf_name[65];
	uint32_t			ip_addr;
	uint32_t			mask;
} qosNasDcmdServerIpInfo_t;

/**
 * This data type is used for show/clr stats requests
 */
typedef struct qosNasDcmdShowOrCltStatsMsg_s
{
	qosNasDcmdCmdcode_t cmdcode;
} qosNasDcmdShowStatsMsg_t;


/**
 * This data typoe is used for stats response data by backend
 */
typedef struct qosNasDcmdStatusRespData_s
{
	uint64_t	pkt_count;
	uint64_t 	byte_cnt;
	qosNasDcmdServerIpInfo_t server_info;
} qosNasDcmdStatusRespData_t;


/**
 * This data type is usedby backend to send status response to plugin
 */

typedef struct qosNasDcmdStatusRespMsg_s
{
	qosNasDcmdCmdcode_t cmdcode;
	uint32_t num_records;
	uint8_t		cos;
	uint8_t		dscp;
	uint8_t		traffic_class;
	bool_t		auto_nas_enabled;
	qosNasDcmdStatusRespData_t data[1];
} qosNasDcmdStatusRespMsg_t;

typedef union QosNasBufferNum_ { 
    struct { 
        u_int16_t   mappedId;
        u_int16_t   tag;     
    } buff;
    u_int32_t   num;   
} QosNasBufferNum_t;

#endif
