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
 *   Author : ranantha/rhkumar                                             *
 **************************************************************************/

#ifndef AG_PLUGIN_TYPES_H
#define AG_PLUGIN_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdbool.h"
#include "stdint.h"
#include "agd/public.h"

#define AG_ENUM_BIT_POS(num)  (1<<num)

#define AG_MAPPINGS 0

typedef enum
{
    AG_CONFIG_CMD_INVALID           = 0,
    AG_CONFIG_CMD_ENABLE            = AG_ENUM_BIT_POS(1),
    AG_CONFIG_GET_ENABLE            = AG_ENUM_BIT_POS(2),
    AG_CONFIG_CMD_CONFIG_ALL        = AG_ENUM_BIT_POS(3),
    AG_SHOW_CMD_SHOWALL             = AG_ENUM_BIT_POS(4),
    AG_SHOW_CMD_MAPSHOW             = AG_ENUM_BIT_POS(5),
    AG_SHOW_CMD_PGSHOW              = AG_ENUM_BIT_POS(6),
    AG_CONFIG_CMD_MAPPING           = AG_ENUM_BIT_POS(7),
    AG_PGADDMODES                   = AG_ENUM_BIT_POS(8),
    AG_PGDELMODES                   = AG_ENUM_BIT_POS(9),
    AG_SHOW_CMD_POLICYSHOW          = AG_ENUM_BIT_POS(10),
    AG_RELCOUNTER_SET               = AG_ENUM_BIT_POS(11),
    AG_STATIC_MAPADD                = AG_ENUM_BIT_POS(12),
    AG_STATIC_MAPDEL                = AG_ENUM_BIT_POS(13)
}AgConfigCmdCode;

typedef enum  {
    AG_INTERFACE_TYPE_FI = 0,
    AG_INTERFACE_TYPE_TE,
    AG_INTERFACE_TYPE_FCOE
} agInterfaceType;

typedef enum
{
    AG_DCM_ERROR = -1,
    AG_DCM_OK,
    AG_DCM_NOT_ENABLED,
    AG_DCM_PG_NOT_ENABLED
}AgStatusCode;

typedef struct {
    int policy;
    int fporttrunkenable;
    u_int fnmtov;
    u_int relcountervalue;
} ag_global_rec_t;

typedef struct {
    int nport;
    BitMap_t fports;
    int failover;
    int failback;
} nportmap_rec_t;

typedef struct {
    int pgid;
    int pgenable;
    char pgname[PGNAME_LEN];
    u_int pgflags;
    BitMap_t nports;
    BitMap_t fports;
} pgmap_rec_t;

typedef struct ag_show_msg_fixed_part_ {
    char            agname[256];
    u_int           policy;
    char            wwn[32];
    char            ipv4_addr[256];
    char            ipv6_addr[256];
    char            fw_version[256];
    int             persistent_alpa[3];
    int             nb_ports;
    int             nb_pg;
    int             nb_fabrics;
    int             nb_nports;
    int             nb_fports;
    int             nb_fport_logins;
} ag_show_msg_fixed_part_t;

typedef struct ag_show_msg_port_group_ {
    u_int           pgid;
    char            pgname[PGNAME_LEN];
    u_int           pgflags;
    BitMap_t        pgmembers_bm;
} ag_show_msg_port_group_t;

typedef struct ag_show_msg_fabric_info_ {
    char            fabric_name[32];
    BitMap_t        nports_bm;
} ag_show_msg_fabric_info_t;

typedef struct ag_show_nport_map_ {
    u_int           nport;
    int             nportID;
    char            pwwn[32];
    char            attached_ipaddr[256];
    int             failover;
    int             failback;
    int             nb_c_fports;
    BitMap_t        fports_bm;
}ag_show_nport_map_t;

typedef struct ag_show_fport_info_ {
    u_int           fport;
    int             fportID;
    char            pwwn[32];
    u_int           curr_nport;
    int             pref_nport;
    int             loginexceed;
} ag_show_fport_info_t;

typedef struct ag_show_nport_static_map_ {
    u_int           nport;
    BitMap_t        static_fports_bm;
} ag_show_nport_static_map_t;

typedef struct ag_mapshow_nport_map_ {
    u_int           nport;
    int             status;
    int             failover;
    int             failback;
    int             pgid;
    char            pgname[PGNAME_LEN];
    BitMap_t        conf_fports_bm;
    BitMap_t        stat_fports_bm;
    BitMap_t        curr_fports_bm;
}ag_mapshow_nport_map_t;

typedef struct ag_pgshow_port_group_ {
    int             pgid;
    int             status;
    char            pgname[PGNAME_LEN];
    u_int           pgflags;
    fnm_state_t     fnm_state;
    BitMap_t        nports_bm;
    BitMap_t        fports_bm;
} ag_pgshow_port_group_t;




typedef struct {
    u_int msg_size;
    ag_global_rec_t ag_cfg;
    int num_nportmap_rec;
    /* This is a placeholder for array of nport records */
    nportmap_rec_t *nport_prop;
    int num_pg_rec;
    /* This is a placeholder for array of portgroup records */
    pgmap_rec_t *pg_prop;
} ag_config_all_msg_t;

typedef struct ag_config_msg_ {
    uint32_t    cmd_code;
    uint32_t    enable;
    uint32_t    status;
} ag_config_msg_t;

typedef struct ag_config_msg_resp_ {
    uint32_t                        cmd_code;
    uint32_t                        size;
    int                             status;
} ag_config_msg_resp_t;


typedef struct ag_static_mapping_msg_ {
    uint32_t     cmd_code;
    int          nport;
    BitMap_t     staticfports;
    uint32_t     status;
} ag_static_mapping_msg_t;

typedef struct ag_static_mapping_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_static_mapping_msg_resp_t;

typedef struct ag_mapping_msg_ {
    uint32_t     cmd_code;
    int          nport;
    BitMap_t     fports;
    uint32_t     status;
} ag_mapping_msg_t;

typedef struct ag_mapping_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_mapping_msg_resp_t;

typedef struct ag_failover_msg_ {
    int  port;
    char enable;
} ag_failover_msg_t;

typedef struct ag_failover_msg_resp_ {
    int32_t                         cmd_code;
    uint32_t                        size;
    uint32_t                        status;
} ag_failover_msg_resp_t;


typedef struct ag_failback_msg_ {
    int  port;
    char enable;
} ag_failback_msg_t;

typedef struct ag_failback_msg_resp_ {
    int32_t                         cmd_code;
    uint32_t                        size;
    uint32_t                        status;
} ag_failback_msg_resp_t;

typedef struct ag_fnmtov_msg_ {
    uint32_t  value;
} ag_fnmtov_msg_t;

typedef struct ag_fnmtov_msg_resp_ {
    int32_t                         cmd_code;
    uint32_t                        size;
    uint32_t                        status;
} ag_fnmtov_msg_resp_t;

typedef struct ag_relcounter_msg_ {
    int32_t                         cmd_code;
    int                             value;
} ag_relcounter_msg_t;

typedef struct ag_relcounter_msg_resp_ {
    int32_t                         cmd_code;
    uint32_t                        size;
    uint32_t                        status;
} ag_relcounter_msg_resp_t;

typedef struct ag_show_msg_ {
    uint32_t    cmd_code;
    uint32_t    nb_elem;
    uint32_t    *elements;
} ag_show_msg_t;

typedef struct ag_show_msg_resp_ {
    u_int           cmd_code;
    u_int           size;
    u_int           status;
    u_int           max_ag_port;
    ag_show_msg_fixed_part_t     ag_info;
    ag_show_msg_port_group_t    *pg_info;
    ag_show_msg_fabric_info_t   *fabric_info;
    ag_show_nport_map_t         *nportmap_info;
    ag_show_fport_info_t        *fport_info;
    ag_show_nport_static_map_t  *nportstatic_info;
} ag_show_msg_resp_t;

typedef struct ag_mapshow_msg_resp_ {
    u_int           cmd_code;
    u_int           size;
    u_int           status;
    u_int           max_ag_port;
    u_int           nb_nports;
    ag_mapshow_nport_map_t  *nport_map;
} ag_mapshow_msg_resp_t;

typedef struct ag_pgshow_msg_resp_ {
    u_int           cmd_code;
    u_int           size;
    u_int           status;
    u_int           max_ag_port;
    u_int           nb_pg;
    ag_pgshow_port_group_t  *pg;
} ag_pgshow_msg_resp_t;

typedef struct ag_pgcreate_msg_ {
    uint32_t     pgid;
    BitMap_t     nports;
    BitMap_t     fports;
    char         pgname[50];
    BitMap_t     pgmodes;
    uint32_t     status;
} ag_pgcreate_msg_t;

typedef struct ag_pgcreate_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_pgcreate_msg_resp_t;

typedef struct ag_pgadd_msg_ {
    uint32_t     pgid;
    BitMap_t     nports;
    uint32_t     status;
} ag_pgadd_msg_t;

typedef struct ag_pgadd_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_pgadd_msg_resp_t;

typedef struct ag_pgdel_msg_ {
    uint32_t      cmd_code;
    uint32_t     pgid;
    BitMap_t     nports;
    uint32_t     status;
} ag_pgdel_msg_t;

typedef struct pgdel_nportmap_ {
    int nport;
    BitMap_t fports;
//    int num_of_fports;
} pgdel_nportmap_t;

typedef struct ag_pgdel_msg_resp_ {
    uint32_t      cmd_code;
    int num_recs;
    uint32_t      size;
    uint32_t      status;
    pgdel_nportmap_t *nport_maps;
} ag_pgdel_msg_resp_t;

typedef struct ag_pgmodes_msg_ {
    uint32_t     pgid;
    BitMap_t     modes;
    uint32_t     action;
    uint32_t     status;
} ag_pgmodes_msg_t;

typedef struct ag_pgmodes_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_pgmodes_msg_resp_t;

typedef struct ag_policy_msg_ {
    agd_policy_t policy;
    int flag;
} ag_policy_msg_t;

typedef struct ag_policy_msg_resp_ {
    int32_t                         cmd_code;
    uint32_t                        size;
    uint32_t                        status;
} ag_policy_msg_resp_t;

typedef struct ag_policyshow_msg_resp_ {
    uint32_t     cmd_code;
    u_int        size;
    uint32_t     status;
    agd_policystatus_t *policystatus;

} ag_policyshow_msg_resp_t;

typedef struct ag_pgrename_msg_ {
    uint32_t     pgid;
    char         name[50];
    uint32_t     status;
} ag_pgrename_msg_t;

typedef struct ag_pgrename_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_pgrename_msg_resp_t;

typedef struct ag_pgremove_msg_ {
    uint32_t     pgid;
    uint32_t     status;
} ag_pgremove_msg_t;

typedef struct ag_pgremove_msg_resp_ {
    uint32_t      cmd_code;
    uint32_t      size;
    uint32_t      status;
} ag_pgremove_msg_resp_t;


#ifdef __cplusplus
}
#endif

#endif

