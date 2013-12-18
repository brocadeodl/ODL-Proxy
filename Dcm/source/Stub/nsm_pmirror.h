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

/* Copyright (C) 2004 IP Infusion, Inc. All Rights Reserved. */

#ifndef _ZEBOS_L2_PMIRROR_H
#define _ZEBOS_L2_PMIRROR_H

#define MAX_MONITOR_DESCR_LEN	64

struct pm_node {
       struct interface *to;
       struct interface *from;
       int    direction;
       int    created;
       int    src_active;
	   int 	  dst_active;
	   int	  desc_valid;
	   char	  desc[MAX_MONITOR_DESCR_LEN];
	   int	  vlanid;
	   int    dst_rb;
	   int    src_rb;
       int    ivid;
};
typedef struct pm_node pm_node_t; 

#define MAX_MONITOR_SESSIONS_CALLISTO_SA 24
#define MAX_MONITOR_SESSIONS_CASTOR_SA 7
#define MAX_MONITOR_SESSIONS_MERCURY_SA 48

#define MAX_MONITOR_SESSIONS_VCS 512

#define RESERVED_TRILL_DA_SPAN_RSPAN 8191

#define RSPAN_INVALID_TO_IFINDEX 0 

#define IFP_GIG 0
#define IFP_TENGIG 1
#define IFP_FORTYGIG 2
#define ENUM_RSPAN_IFP_TYPE 3
#define ENUM_DEFAULT_IFP_TYPE 4

extern struct pm_node sessions[MAX_MONITOR_SESSIONS_VCS];

void port_mirroring_init(struct lib_globals *zg);
void port_mirroring_deinit();
int port_mirroring_write(struct cli *cli);
int port_mirroring_unconfig(struct interface *);
int port_mirroring_unconfig_intf(struct interface *);
int nsm_rbridge_unreachable_update(int rbridge);
int max_sessions();

typedef enum nsm_span_type_ {
	NSM_SPAN_STANDALONE = 1,
	NSM_SPAN_IN_VCS_SRC_CONFIG = 2,
	NSM_SPAN_IN_VCS_INT_CONFIG = 4,
	NSM_SPAN_IN_VCS_DST_CONFIG = 8,
	NSM_SPAN_RSPAN_CONFIG = 16
} span_type_t;

typedef enum span_error_code_ {
    /* starting enum value from 100 to avoid any conflict */

	/* span event */
	    NSM_SPAN_PORT_MIRROR_NOT_DELETE = 100,
		NSM_SPAN_INTERFACE_DOES_NOT_EXIST,
		NSM_SPAN_SOURCE_DEST_PORT_SAME,
		NSM_SPAN_DEST_PORT_IN_PROTOCOL_OR_LAG,
		NSM_SPAN_INCORRECT_DIRECTION,
		NSM_SPAN_ARGUMENTS_NOT_MATCH_CONFIGURATION,
		NSM_SPAN_MONITOR_SESSION_ALREADY_CONFIGURED,
		NSM_RSPAN_MONITOR_SESSION_SOURCE_ALREADY_CONFIGURED,
		NSM_SPAN_SOURCE_ALREADY_CONFIGURED_SAME_DIR,
		NSM_SPAN_SOURCE_ALREADY_CONFIGURED_AS_DESTINATION,
		NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_SOURCE_PORT,
		NSM_SPAN_DESTINATION_ALREADY_CONFIGURED_AS_DESTINATION_ANOTHER_SESSION,
		NSM_SPAN_HARDWARE_PORT_CONFIGURATION_FAILED,
		NSM_SPAN_DESCRIPTION_LENGTH_MORE_THAN_LIMIT,
		NSM_SPAN_SESSION_DOESNOT_EXIST,
		SPAN_MIRRORING_ISL_INTERFACE_COMMAND_ERROR
} span_error_code_t;

#endif /* _ZEBOS_L2_PMIRROR_H */
