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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __MQUEUE_H__
#define __MQUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <hasm/smi.h>

#define MSG_SCN 1

typedef struct {
	char		mq_name[24];	/* queue name */
	int		mq_max;		/* max # of messages */
	int		mq_size;	/* max message size */
	int		mq_opt;		/* queue option */
	int		mq_hwm;		/* high water mark */
	int		mq_missed;	/* miss count */
	void		*mq_wakeup_arg;	/* wakeup function arg */

} msgQ_t;

typedef	msgQ_t	*mq_t;
static msgQ_t tmpMsgQ_t;

#define ASP_FLG_NON_FSS_CLIENT  1
#define ASP_FLG_FCSWITCH        2
#define WAIT_FOREVER            3
#define MSG_Q_FIFO              4

#define MSG_MAX                 50

//  MSG_SCN_SW_DOMAIN(msg) is defined in fabos/mace/cnm/include/mqueue.h as: MSG_SCN_SW_DOMAIN(msg)	((msg)->msg_scn.scn_data_t.switch_scn.domain)
// code that populates these in fabos is: /vobs/projects/springboard/fabos/src/sys/dev/switch/switch_ioctl.c
#define MSG_SCN_SW_DOMAIN(msg)  1 // faked out here for x86 in switch env this is the domainId for the SCN
#define MSG_SCN_SW_ARG1(msg)	1 // faked out here for x86 in the switch env this is the domainId for the fcPrincipal
#define MSG_SCN_PORT_NO(msg) 1 

#define SCN_END_MARKER          (255)
#define MSG_SCN_PORT_IFID(msg) 1

#define ASP_FLG_SINGLE_THREAD   0x00000001
#define ASP_FLG_SYSMOD          0x00000200

#define FSS_CFLG_DLOG           1
#define FSS_RECOV_ACTIVE    1001
#define FSS_RECOV_COLD      1002
#define FSS_RECOV_WARM      1003

#define FSS_STATUS_SYNFAIL  4

// slot scns
#define sysScnObject(pmsg_scn) 1
#define sysScnUnitNum(pmsg_scn) 1
#define sysScnHwType(pmsg_scn) 1
#define sysScnHwId(pmsg_scn) 1
#define sysScnVersion(pmsg_scn) 1
#define sysScnReason(pmsg_scn) 1

#define SCN_GET_MODID(reason) 1
#define SCN_GET_REASON(reason) 1

#define SCN_NORMAL 1 
#define SCN_SKIP_POST 1 
#define SCN_SKIP_POST_AP 1 
#define SCN_FAIL_OVER 1 
#define SCN_FAIL_OVER_AP 1 
#define SCN_MOD_EM 1 
#define SCN_MOD_BLADE 1 
#define SCN_MOD_DIAG 1 
#define SCN_OCP_READY 1    /* 0x14 SYSMOD/EM - other CP drop error */ 
#define SCN_CP_FAULT 1     /* 0x35 SYSMOD/EM error on cp-section */

enum {
    SWITCH_SCN,
    SW_ONLINE,
    SW_OFFLINE,
    DOMAIN_VALID,
    DOMAIN_INVALID,
    DOMAIN_CAP_AVAILABLE,
    DOMAIN_REACHABLE,
    DOMAIN_UNREACHABLE,
    DOMAIN_JOIN_COMPLETE,
    SLOT_SCN,
    RSCN,
	SW_DISABLES_PORT,
	ZONE_CHANGED,
	ZONE_ABORTED,
    SWITCH_READY_TO_ACCEPT_COMMANDS,
    SWITCH_BOOTUP_STATE_UPDATE,
    SWITCH_STATE_READY_ACCEPT_COMMANDS,
    SWITCH_STATE_ACTIVATION_COMPLETE,
	SLOT_ON,
	SLOT_IN,
	SLOT_OUT,
	SLOT_FAULT,
    SLOT_READY
};

typedef struct msg {
    u_char  msg_type;   /* message type - */ 
    struct
    {
        long type;
        long subtype;
    }msg_scn;
	void *msg_data;
	void *msg_app_data;
	void *to_arg1;
} msg_t;

/* msg_type values */
enum
{
    MSG_IPC         /* Dcmd IPC messages */
};

/* sema related defs */
typedef struct sema { 
    int         type;
    int         v;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
    pthread_condattr_t  attr;
} sema_struct;

typedef sema_struct *sema_t;

/*
 * special values for semaCreate()
 */
#define SEM_COUNT   -1  /* empty counting semaphore */
#define SEM_EMPTY   0   /* empty binary semaphore */
#define SEM_FULL    1   /* full binary semaphore */

static inline sema_t  semaCreate(int count)
{
	sema_t sema = NULL;

	return (sema);
}

static inline int semaDelete(sema_t sema)
{
	return (0);
}
static inline int semaGive(sema_t sema)
{
	return (0);
}
static inline int semaTake(sema_t sema, int timeout)
{
	return (0);
}


static inline int asp_init(char *name, int flag, int (*initf)(void), int (*exitf)(void))
{
    return (0);
}

static inline int asp_main(void (*serve)(msg_t *))
{
    while (true) {sleep (5);}
    return (0);
}

static inline void asp_exit(void)
{
}

static inline int aspInit(char *name, int inst, int (*initf)(void), int (*exitf)(void))
{
    return (0);
}

static inline int aspMainLoop(int (*serve)(void))
{
    while (true) {sleep (5);}
    return (0);
}

static inline void ASPExit(void)
{
}

static inline void ASPClose(void)
{
}

static inline int scnRegister(u_int scn_types[][2], void (*callback)(void *), mq_t msg_q, int sw_no)
{
    return (0);
}

static inline mq_t mqCreate(char *name)
{
    return (&tmpMsgQ_t);
}

static inline int mqRead(mq_t mq, msg_t *msg, int timeout)
{
    return (0);
}

static inline int mqWrite(mq_t mq, msg_t *msg)
{
	return (0);
}

static inline int msgQDelete(mq_t)
{
    return (0);
}

static  inline  int mqDelete(mq_t mq)
{
    return (msgQDelete(mq));
}

static inline mq_t msgQCreate(int max, int mlen, int opt, char *name)
{
    return (&tmpMsgQ_t);
}

static inline int msgQReceive(mq_t mq, char *buf, u_int size, int timeout)
{
    return (0);
}

/* FSS Implementation stuff */
typedef struct  {
    char    sg_sname[16];    /* service name */
    char    sg_iname[16];    /* service instance name */
    int sg_no;          /* segment number */
} sgid_t;   

typedef struct  smi_cfg {
    int sc_flags;   /* flags, not used, set to 0 */
    int sc_dist;    /* distributed segment patter */
    union {
        pbmp_t  ports;  /* port bitmap to service instance */
        pbmp_t  delta;  /* changed portmap for expand/deexpand */
    } u;
#define sc_ports    u.ports
#define sc_delta    u.delta
} smi_cfg_t;

typedef struct  {
    int se_event;   /* event id, SMI_EVT_XXX */
    int se_slot;    /* slot# */
    int se_part;    /* partition# */
    int se_arg;     /* event argument (may optional) */
} smi_evt_t;


typedef struct  {
    sgid_t      ai_sgid;
#define ai_sname    ai_sgid.sg_sname
#define ai_iname    ai_sgid.sg_iname
#define ai_sgno     ai_sgid.sg_no
    char        ai_comp[16];
} said_t;

typedef struct  {
    char    ci_sname[16];    /* service name */
    char    ci_iname[16];    /* service instance name */
    char    ci_comp[16]; /* component name */
} scid_t; 

typedef struct  smi_reg {
    int sr_flag;    /* not used, set to 0 */
    int   (*sr_ctor)(said_t *aid, mq_t mq, smi_cfg_t *config);
    int   (*sr_dtor)(said_t *aid);
    int   (*sr_expd)(said_t *aid, smi_cfg_t *config);
    int   (*sr_dxpd)(said_t *aid, smi_cfg_t *config);
    int   (*sr_notify)(said_t *aid, smi_evt_t *evt);
} smi_reg_t;

static inline int smi_register(scid_t *cid, smi_reg_t *reg) 
{
    return (0);
}




/*static	fssd_ops_t dcm_ops = {
		op_recover:	dcm_recover,
		op_recv_update:	dcm_recv_update,
		op_send_dump:	dcm_send_dump,
		op_recv_dump:	dcm_recv_dump
	};

static	fssd_stg_t dcm_stgs[] = {
		{ fs_stagenum:	DCM_RECOV_PUSH,
		  fs_stagename:	"DCM_RECOV_PUSH" },
	};

static	fssd_reg_t dcm_fss = {
		fr_ops:		&dcm_ops,
		fr_nstages:	0,
		fr_stages:	NULL,
		fr_cflags:	0
	};*/

typedef struct  fssd_udb {
    int     fu_cnt;     /* # of buffers in vector */
    struct  iovec  *fu_iov;     /* io vector */
    uint        fu_odata;   /* operation data */
    uint        fu_flags;   /* flags (ACK/TXEND) */
    int     fu_tid;     /* transaction id */
    int     fu_sgno;    /* segment id */
}
fssd_udb_t;

typedef struct  fssd_stg {
    int fs_stagenum;        /* [1 - 999] for client stages */
    char    fs_stagename[32];
}
fssd_stg_t;

typedef struct  fssd_ops {
    int   (*op_recover)(void *ctxt, int stage);
    int   (*op_recv_update)(void *ctxt, fssd_udb_t *updt);
    int   (*op_send_dump)(void *ctxt, int vers, int tid);
    int   (*op_recv_dump)(void *ctxt, int vers, int sgno);
}
fssd_ops_t;

typedef struct  fssd_reg {
    said_t      fr_said;    /* service instance atom */
#define fr_sgid    fr_said.ai_sgid  /* service segment */
#define fr_sname   fr_said.ai_sname /* service type name */
#define fr_iname   fr_said.ai_iname /* service instance name */
#define fr_comp    fr_said.ai_comp  /* service component name */
#define fr_sgno    fr_said.ai_sgno  /* service segment id */
    void           *fr_ctxt;    /* callback context */
    fssd_ops_t     *fr_ops;     /* callback ops */
    int     fr_nstages; /* stage count */
    fssd_stg_t     *fr_stages;  /* stage table */
    int     fr_cflags;  /* see FSS_CFLG_XXXX */
}
fssd_reg_t;

static inline int fssd_register(fssd_reg_t *reg, mq_t mq)
{
    return (0);
}

static inline int fssd_send_update(int fssh, fssd_udb_t *udb)
{
    return (0);
}

static inline void fssd_notify_status(int fssh, int type, int status)
{
    return;
}

static inline int hasm_setpid(said_t *aid, int pid)
{
    return (0);
}

static  inline void FSSD_DUMP_BEGIN(int fssh) {
}

static  inline void FSSD_DUMP_END(int fssh, int status) {
}

#ifdef __cplusplus
}
#endif

#endif // __MQUEUE_H__
