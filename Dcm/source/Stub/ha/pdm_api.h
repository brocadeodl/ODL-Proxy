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
 *    Copyright (c) 1996-2007 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *  Header for PDM Shared Library API.
 *
 */
#ifndef __PDM_API_H__
#define __PDM_API_H__
#if 0
#include <sys/fabos/cdefs.h>

OS_CDECLS_BEGIN

#include <sys/fablog/fabmod.h>
#include <sys/fablog/fabmod.h>
#include <sys/fablog/faberr.h>
#include <sys/fablog/debug.h>

/* Different Blade Modes macro definitions. 
 * This is being shared by PDM and IPS. 
 */
#define PDM_BLADE_GEMODE_1G     0
#define PDM_BLADE_GEMODE_10G    1
#define PDM_BLADE_GEMODE_DUAL   2
#define PDM_BLADE_GEMODE_XPORT  3 /* Crossport mode for DP only */

#define PDMIPCNAME  "PDMIPC"    /* for chassis pdm */
#define PDMIPCNAME_S    "PDMIPC_S"  /* for fcsw pdm */

#define PDM_IPC_NONE            0
#define PDM_IPC_OUTPUT_REDIRECT 1
#define PDM_IPC_MAX             2

#define PDM_MAX_FILES       40  /* legacy max limit */
#define PDM_MAX_SIZE        50  /* max # of bytes of file name */

/*
 * PDM message defined for IPC. DO NOT change this data structure due to
 * the backward compatibility requirement.
 */
typedef struct pdm_save {
    int pm_count;       /* number files filled below */
    char    pm_list[1][PDM_MAX_SIZE]; /* file list */
} pdm_save_t;

#define PDM_REQ_NONE            0
#define PDM_REQ_PCFG_COMMIT     1
#define PDM_REQ_UPDATE          2
#define PDM_REQ_PCFG_GET        4
#define PDM_REQ_PCFG_SET        5
#define PDM_REQ_PCFG_DEFAULT        6
#define PDM_REQ_SWD_KICKIN      7
#define PDM_REQ_CFG_GET         8
#define PDM_REQ_CFG_SET         9
#define PDM_REQ_CFG_REMOVE      10
#define PDM_REQ_CFG_QUERY       11
#define PDM_REQ_CFG_MATCH       12
#define PDM_REQ_FSS_FAILURE     13

#define PDM_REQ_BLADE_MODE_SET  20
#define PDM_REQ_BLADE_MODE_GET  21

/* flags for portcfg_get/set */
#define PDM_OPT_NONE            0x0
#define PDM_OPT_PCFG            0x0
#define PDM_OPT_NO_COMMIT       0x01
#define PDM_OPT_LSPCFG          0x02
#define PDM_OPT_DEFAULT         0x04
#define PDM_OPT_EMPTY_PORT      0x08
#define PDM_OPT_PRESET          0x10


#define MAX_CONFIG_SIZE     (1024 * 32)
typedef struct _pcfg_msg_t {
    int             flag;           /* flag */
    pcfg_bm_t       bm;             /* port bitmap */
} pcfg_msg_t;

typedef struct _cfg_remote_t {
    int flag;
    int status;
    char    key[128];
    int type;
    int size;
    char    data[MAX_CONFIG_SIZE + 8];  /* 32k max */
} cfg_remote_t;

typedef struct  {
    int     mask;       /* config bit # */
    int     val;        /* bitmap val with specified mask */
} ge_pcfg_msg_t;

/*
 * portcfg access, centralized in pdm context.
 */
extern  int portcfg_set(int port, pcfg_bm_t *bm, int flag);
extern  int portcfg_get(int port, pcfg_bm_t *bm, int flag);
extern  int portcfg_commit(void);

extern  int ge_portcfg_set(int port, int val, int mask);
extern  int ge_portcfg_get(int port, int *val, int mask);
extern  int ge_portcfg_commit(void);

extern  void    config_swd_kickin(void); /* test swd */
extern  void    config_fss_failure(void); /* test fss */
extern  int ge_blademode_set(int slot, int mode);
extern  int ge_blademode_get(int slot, int *val);

/*
 * service types
 */
#define PDM_SVC_CHASSIS 0
#define PDM_SVC_ETHSW   1
#define PDM_SVC_FCSW    2

/*
 * config files sync-up onto the secondary partition and the standby.
 */
extern  int config_save(int service, char *filv[]);
extern  int config_update(void);

/*
 * the following api's are going to deprecated, please do not use
 * directly any more!!!
 */
extern  int configSync(void);
extern  int configChassisUpdate(void);
extern  int configSave(char *filv[]);
extern  int configUpdate(void);

OS_CDECLS_END
#endif
inline static int configUpdate(void) { return 0; }
inline static int configSave(char *filv[]) { return 0; }
#endif /* __PDM_API_H__ */
