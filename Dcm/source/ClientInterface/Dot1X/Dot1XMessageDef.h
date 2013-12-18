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

#ifndef __DOT1X_MESSAGE_DEF_H__
#define __DOT1X_MESSAGE_DEF_H__

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/InterfaceCommon.h"
#define MAX_IF_NAME_LEN 64
#define PACKED __attribute__((packed))

#define DEBUG_FLAG_TX          (1 << 4)
#define DEBUG_FLAG_RX          (1 << 5)
#define DEBUG_FLAG_DETAIL      (1 << 6)
#define DEBUG_FLAG_RXTX            (DEBUG_FLAG_TX|DEBUG_FLAG_RX)

/* Tag Names */
typedef enum
	{
  		DOT1XSHOW,
  		DOT1XSHOWALL,
  		DOT1XSHOWINTERFACE,
  		DOT1XSHOWINTERFACE_ALL,
  		DOT1XSHOWSTATS,
  		DOT1XSHOWSESSIONINFO,
  		DOT1XSHOWDIAG
	} Dot1XMessageBufferId;

/* show Dot1X cmd code */
typedef enum
    {
        DOT1X_CMD_SHOW,
        DOT1XSHOW_ALL,
        DOT1XSHOW_INTERFACE,
        DOT1XSHOW_STATS,
        DOT1XSHOW_SESSION_INFO,
        DOT1XSHOW_DIAG,
		DOT1XCLEAR_INTERFACE,
		DOT1XCLEAR_ALL,
		DOT1XREAUTHENTICATE_INTERFACE,
		DOT1XTESTEAPOL_CAPABLE_ALL,
		DOT1XTESTEAPOL_CAPABLE_INTF,
		DOT1X_RADIUS_CONFIG_ADD,
		DOT1X_RADIUS_CONFIG_CHNG,
		DOT1X_RADIUS_CONFIG_DEL,
		DOT1XDEBUG_CMD,
		DOT1X_CMDCODE_MAX
	} Dot1XShowIntfCmdCode;




/* show Dot1X cmd code */
typedef enum
    {
		DEBUG_AUTH = 1,
		DEBUG_NO_AUTH,
		DEBUG_SHOW,
		DEBUG_INTERFACE_CMD,
		DEBUG_ALL_CMD
	} Dot1XDebugCmdCode; 

typedef struct Dot1XShow_InputMsg_t_ {

    char if_name[64];
    char interface[64];
    unsigned int cmdCode;
} Dot1XShow_InputMsg_t;


typedef struct Dot1XDebug_InputMsg_t_ {
    char if_name[MAX_IF_NAME_LEN+4];
    char interface[MAX_IF_NAME_LEN+4];
    unsigned int cmdType;
    unsigned int cmdCode;
    unsigned int subCode;
} Dot1XDebug_InputMsg_t;

typedef struct Dot1XReauthenticate_InputMsg_t_ {

    char if_name[MAX_IF_NAME_LEN+4];
    char interface[MAX_IF_NAME_LEN+4];
    unsigned int cmdCode;
} PACKED Dot1XReauthenticate_InputMsg_t;


typedef struct Dot1XRadius_InputMsg_t_ {
	char hostname[MAX_IF_NAME_LEN+4];
    char secret[MAX_IF_NAME_LEN+4];
    int timeout;
    int retries;
    unsigned short port;
    unsigned char  proto;
    unsigned int cmdCode;
} PACKED Dot1XRadius_InputMsg_t;

typedef struct Dot1XTest_InputMsg_t_ {
    char if_name[MAX_IF_NAME_LEN+4];
    char interface[MAX_IF_NAME_LEN+4];
    unsigned int cmdCode;
} PACKED Dot1XTest_InputMsg_t;

typedef struct dot1x_config_Radius_ {
    int configured;
    char server_string[64+1];
    char sin_port[64+1];
    char shared_secret[64 +1];
    unsigned int timeout;
} dot1x_config_Radius_t;

typedef struct Dot1X_show_dot1x_output_msg_ {
    char status[30];
	int num_radius;
	dot1x_config_Radius_t config[1];
} Dot1X_show_dot1x_output_msg_t;

typedef struct dot1x_config_Inputmsg_ {
    char ifname[64];
    unsigned int enabled;
	unsigned int port_ctrl_type;
	unsigned int proto_version;
	unsigned int quiet_interval;
	unsigned int max_reauth_attempt;
	unsigned int enable_auth;
	unsigned int reauth_timeout_interval;
	unsigned int server_timeout_interval;
	unsigned int supp_timeout_interval;
	unsigned int tx_timeout_interval;
	unsigned int cmdCode;
	unsigned int opCode;
	unsigned long iftype;
} dot1x_config_Inputmsg_t;

typedef enum
    {
	  DOT1X_SET_OP,
	  DOT1X_CREATE_OP,
	  DOT1X_DELETE_OP
	} Dot1xOp;

typedef enum
    {
	  DOT1X_ENABLED,
	  DOT1X_PORT_CTRL_TYPE,
	  DOT1X_PROTO_VERSION,
	  DOT1X_QUIET_INTERVAL,
	  DOT1X_MAX_REAUTH_ATTEMPT,
	  DOT1X_ENABLE_AUTH,
	  DOT1X_REAUTH_TIMEOUT_INTERVAL,
	  DOT1X_SERVER_TIMEOUT_INTERVAL,
	  DOT1X_SUPP_TIMEOUT_INTERVAL,
	  DOT1X_TX_TIMEOUT_INTERVAL,
	  DOT1X_OPCODE_ALL
	} Dot1xParams;


typedef struct Dot1X_show_interface_output_msg_ {

  	char port_mode_str[20];
  	char port_mode_name[16];
    char port_ctl[20];
    char port_auth_status[16];
	unsigned int proto_version;
    char reauth[30];
    unsigned int auth_fail_max;
    unsigned int reauth_max;
    unsigned int tx_period;
    unsigned int quiet_period;
    unsigned int supp_timeout;
    unsigned int server_timeout;
    unsigned int reauth_interval;
    char pae_state[30];
    char be_state[16];
    char last_supp_name[63+1];
    char last_supp_address[16];
    unsigned int current_id;
    unsigned int if_from_server;
} Dot1X_show_interface_output_msg_t;

typedef struct Dot1X_show_diag_output_msg_ {
  	char mode_str[32];
  	char mode_name[32];
    unsigned int authEnterConnecting;
    unsigned int authEaplogoffWhileConnecting;
    unsigned int authEnterAuthenticating;
    unsigned int authSuccessWhileAuthenticating;
    unsigned int authTimeoutWhileAuthenticating;
    unsigned int authFailWhileAuthenticating;
    unsigned int authEapstartWhileAuthenticating;
    unsigned int authEaplogoffWhileAuthenticating;
    unsigned int authReauthsWhileAuthenticated;
    unsigned int authEapstartWhileAuthenticated;
    unsigned int authEaplogoffWhileAuthenticated;
    unsigned int BackendResponses;
    unsigned int BackendAccessChallenges;
    unsigned int BackendOtherrequestToSupplicant;
    unsigned int BackendAuthSuccess;
    unsigned int BackendAuthFails;
} Dot1X_show_diag_output_msg_t;

typedef struct Dot1X_show_session_info_output_msg_ {

  	char mode_str[32];
  	char mode_name[32];
    char username[65];
    char session_time[128];
    char session_status[128];
} Dot1X_show_session_info_output_msg_t;

typedef struct Dot1X_show_stats_output_msg_ {

  	char mode_str[32];
  	char mode_name[32];
    unsigned int eapol_rx;
    unsigned int eapol_tx;
    unsigned int eapol_start_rx;
    unsigned int eapol_logoff_rx;
    unsigned int eap_rsp_id_rx;
    unsigned int eap_resp_rx;
    unsigned int eap_req_id_tx;
    unsigned int eap_req_tx;
    unsigned int invalid_eapol_rx;
    unsigned int eapol_length_error_rx;
    unsigned int eapol_last_frame_version;
    unsigned int invalid_eap_rx;
    unsigned int eap_length_error_rx;
    char eapol_last_frame_src[64];

} Dot1X_show_stats_output_msg_t;


typedef struct Dot1X_show_msg_ {
    unsigned int num_records;
    unsigned int record_type;
    unsigned int enabled;
    unsigned int readinesstesttimeout;
	Dot1X_show_dot1x_output_msg_t info[1];
    union {
        Dot1X_show_interface_output_msg_t interface[1];
        Dot1X_show_diag_output_msg_t diag[1];
        Dot1X_show_session_info_output_msg_t session[1];
        Dot1X_show_stats_output_msg_t stats[1];
    } records;
} Dot1X_show_msg_t;

typedef struct Dot1X_get_msg_ {
    unsigned int num_records;
    unsigned int record_type;
    union {
        dot1x_config_Inputmsg_t interface[1];
    } records;
} Dot1X_get_msg_t;


typedef struct Dot1X_timeout_global_config_msg_ {
	int	timeout;
} Dot1X_global_timeout_config_msg_t;

typedef struct Dot1X_mac_global_config_msg_ {
	int	cmdcode;
} Dot1X_mac_global_config_msg_t;





#endif
