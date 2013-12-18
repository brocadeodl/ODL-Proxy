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

#ifndef ISC_STUB_H
#define ISC_STUB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <utils/mqueue.h>
typedef struct	isc_msg {
	int im_type;	/* message type */
	int im_oid;		/* object identity */
	int im_cop;		/* component optional parameter, traced */
	int im_size;	/* message size */
	void	*im_msgp;	/* message data */
} isc_msg_t;

typedef struct	isc_evt {
	int ie_evt;     /* event id */
	int ie_nid;     /* peer node id */
	int ie_arg;     /* argument, not used */
} isc_evt_t;


typedef struct	isc_ops {
	int	(*op_recv_request)(void *ctxt, isc_msg_t *msg, int reqh);
	int	(*op_recv_reply)(void *ctxt, isc_msg_t *msg, void *reqx);
	int	(*op_recv_event)(void *ctxt, isc_evt_t *evt);
} isc_ops_t;

typedef struct	isc_reg {
	said_t	ir_said;	/* component identity, informational */
	void	*ir_ctxt;	/* callback context */
	isc_ops_t	*ir_ops;	/* callbacks */
	int		ir_flag;	/* control flag, see ISC_CFLG_XXX */
} isc_reg_t;

typedef struct	isc_exp {
	int ex_a;
	int ex_b;	/* for oid range [a,b] match */
} isc_exp_t;

typedef struct	isc_tok {
	int		it_type;		/* message type to be subscribed */
	int		it_flag;		/* bit-wise control flags */
	isc_exp_t	it_oids;	/* object expression */
} isc_tok_t;

/*
 * bit-wise control flag
 */
#define ISC_CFLG_LOCAL	0x0001		/* intra-node domain */
#define ISC_CFLG_IMME	0x0002		/* return on error, for non-daemon */
#define ISC_CFLG_CLI	0x0003		/* cli mode, non-daemon, LOCAL+IMME */
#define ISC_CFLG_PART	0x0004		/* in partition context */
#define ISC_CFLG_EVENT	0x0008		/* event indication required */
#define ISC_CFLG_DLOG	0x0010		/* debug log enabled */
#define ISC_CFLG_HA		0x0020		/* ha auto-transition */
#define ISC_CFLG_LO		0x0040		/* loopback enabled */
#define ISC_CFLG_FWD	0x0080		/* no subscription, forward to bridge */

enum	{
	ISC_DOM_NONE		= 0,	/* not used */
	ISC_DOM_TEST		= 1,	/* internal test */
	ISC_DOM_HASM		= 2,	/* hasm startup */
	ISC_DOM_RASLOG		= 3,	/* raslog message */
	ISC_DOM_RASC		= 4,	/* ras comunication message */
	ISC_DOM_CHASSIS		= 8,	/* in case multiple chassis instances */
	ISC_DOM_FCSW		= 16,	/* 32 partitions reserved */
	ISC_DOM_FCR			= 49,	/* FCR */
	ISC_DOM_SUPPORTSAVE = 50,	/* System-wide SupportSave data */
	ISC_DOM_IPC			= 100,	/* for IPC internal usage */
	ISC_DOM_ETHSW		= 101,	/* please reserve 32 just in case */
	ISC_DOM_KI			= 200,	/* for KI internal usage */
	ISC_DOM_CDC			= 201,	/* for CDC proxy messages */
	ISC_DOM_FCIP		= 202,	/* for FCIP proxy messages */
	ISC_DOM_DIAG		= 203	/* for DIAG test message */
};

/*
 * list of message types in domain chassis and fcsw
 */
enum	isc_service_type_e {
	ISC_TYPE_PDM_SAVE		= 1,	/* pdm config save (chassis/fcsw) */
	ISC_TYPE_PDM_REQUEST	= 2,	/* pdm config request (fcsw) */
	ISC_TYPE_PDM_PORTCFG	= 3,	/* pdm portcfg request (fcsw) */
	ISC_TYPE_HASM_API		= 4,	/* hasm messages */
	ISC_TYPE_CONFIG			= 5,	/* config up/download request */
	ISC_TYPE_SUPPSAVE_DATA	= 6,	/* supportSave Data messages */
	ISC_TYPE_PDM_GE_PORTCFG = 7,	/* GE portcfg request */
	ISC_TYPE_EM_API			= 8,	/* em messages */
	ISC_TYPE_QUERY_SLOT		= 9,	/* dcm messages */
	ISC_TYPE_IPC_MSG		= 20,	/* IPC message */
	ISC_TYPE_IPC_CTRL		= 21,	/* IPC registration message */
	ISC_TYPE_KIR_DRV_OPEN	= 22,	/* KI REM DRV_OPEN */
	ISC_TYPE_KIR_DRV_CLOSE,
	ISC_TYPE_KIR_DRV_REQ_REG,
	ISC_TYPE_KIR_DRV_REGISTER,
	ISC_TYPE_KIR_DRV_ACTION,
	ISC_TYPE_KIR_ASYNC,
	ISC_TYPE_KIR_SCN_DATA,
	ISC_TYPE_KIR_SCN_CONTROL,
	ISC_TYPE_KIR_SCN_REFRESH_REQ,
	ISC_TYPE_KIR_COLLECT
};

static inline int isc_register(int domain, isc_reg_t *reg, mq_t mq) {
	return (0);
}

static inline int isc_subscribe(int isch, int num, isc_tok_t tok[]) {
	return (0);
}

static inline int isc_send_reply(int isch, isc_msg_t *msg, int reqh) {
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif //ISC_STUB_H
