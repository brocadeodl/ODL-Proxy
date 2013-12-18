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
 *
 *    Copyright (c) 2007 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *		Structures for the FCR ioctls called by the hardware dependent layer
 *		of fcrd.
 *
 */
#ifndef __FCR_IOCTL_H__
#define __FCR_IOCTL_H__

/*
 * Structures for FCR operations.
 *
 */
/* Used by IOC_FCR_SET_SFID */
typedef struct _fcrSfid {
#define DEFAULT_BB_FID	1
	uint		fabric_id;		   	/* source fabric id */
	uint 		ex_pid;				/* SRP for the EX port */
} fcrSfid_t;

/* Used by IOC_FCR_ADD_EDGE_DOM and IOC_FCR_DEL_EDGE_DOM */
typedef struct _fcrDomain {
	uint		proxy_domain;   	/* front/translate Domain */
	uint		dfid;				/* destination fabric id */
	uint		fd_flag;			/* 1 if the domain is a front domain */
#define FD_FLAG_TRANSLATE	0		/* for translate domain */
#define FD_FLAG_FRONT		1		/* for front domain */
#define FD_FLAG_LE			2		/* for LE domain */
	uint		force;				/* indicates MUST do operation */
} fcrDomain_t;

/* Used by IOC_FCR_ADD_FAB_ROUTE and IOC_FCR_DEL_FAB_ROUTE */
typedef struct _fcrFabRoute {
	uint 		dfid;				/* destination fabric id */
	uint		last_drp_deleted;	/* flag set by driver */
	uint 		num_drps;			/* # of DRP in list */
	uint		*drp_list;	   	/* DRP list */
} fcrFabRoute_t;

/* Diverse from of fcrFabRoute_t, used by CP and DPF in FCIP (Striker) */
typedef struct _fcip_drp_s {
	uint		is_local;
	uint		is_chip;
	uint		drp;
} fcip_drp_t;

typedef struct _fcipFcrFabRoute {
	uint 		dfid;				/* destination fabric id */
	uint		num_drps;			/* # of local + # of remote DRP in list */
	uint 		num_local_drps;		/* # of local DRP in list */	
	uint		num_remote_drps;	/* # of remote DRP in list */
	fcip_drp_t	*drp_list;	/* DRP list: local first then remote */
} fcipFcrFabRoute_t;

/* Used by IOC_FCR_ADD_PROXY */
typedef struct _fcrProxy {
	uint		proxy_id; 			/* proxy device ID */
	uint	   	dfid;				/* destination fabric ID */
	uint	   	device_id;			/* real device ID */
#define	FCR_TI_PROXY_MASK			0x80000000
#define	FCR_HA_PROXY_MASK			0x40000000
	uint		app_flag;			/* flag to indicate application usage */
	uint		srp;				/* SRP of TI to be used */
	uint		nhop;				/* User port # of TI to be used */
	uint		drp;				/* DRP of TI to be used */
} fcrProxy_t;

/* Wrapper of fcrProxy_t, used by CP and DPF in Striker */
typedef struct _fcipFcrProxy {
	fcrProxy_t	proxy;
	uint		sfid;
} fcipFcrProxy_t;

/* Wrapper for le domain ioctl, used by CP and DPF in Striker to fwd C2 EX port info */
typedef struct _fcipFcrLeDomain {
	uint32_t	le_domain;
	uint32_t	sfid;
} fcipFcrLeDomain_t;

/* Used by IOC_FCR_ADD_EDGE_ROUTE */
typedef struct _fcrEdgeRoute {
	uint		fabric; 			/* fabric */
	uint	   	domain;				/* domain */

	struct {
		uint    pass_through:1;			/* striker c-port pass through flag */
        } flags;

} fcrEdgeRoute_t;

/* Used by IOC_FCR_EX_PORT_CFG_UPDATE */
typedef struct _fcrExPortCfg {
     uint8_t	update_mask;	/* Indicates what fields have valid values */	
     uint8_t	pid_fmt;	/* pid fmt : core, native,displaced */ 
     uint16_t	fid;		/* Fabric id */
 
     uint32_t	r_a_tov;	/* Fabric params R_A_TOV and E_D_TOV */
     uint32_t	e_d_tov;

     uint8_t	front_domain;	/* Front Domain ID */
     wwn_t	front_wwn;	/* Front Domain wwn */
 
     uint8_t	p_domain;	/* Principal switch domain id */
     wwn_t	p_wwn;		/* Principal switch wwn */



     wwn_t	edge_wwn;	/* Edge switch wwn */
     uint8_t	edge_name[64];  /* Edge switch name */
 } fcrExPortCfg_t;

typedef	struct	port_fabid	{
	int			p_number;
	uint16_t	p_fabid;
} port_fabid_t;

/* Used by IOC_FCR_SET_BB_FAB_ID */
typedef struct _fcrBBCfg {
	uint		fabric_id;		   	/* BB fabric id */
	int 		fcr_state;			/* FCR state */
} fcrBBCfg;

/* Possible values for above update_mask */
#define EX_PORT_CFG_ADMIN	0x01   /* Updates : pid_fmt, fid, r_a_tov, e_d_tov */
#define EX_PORT_CFG_PR_SWITCH	0x02   /* Updates : Pr. Switch parameters : p_domain, p_wwn */
#define EX_PORT_CFG_FR_PHANTOM	0x04   /* Updates : front phantom params : front_domain, front_wwn */
#define EX_PORT_CFG_EDGE_SWITCH	0x08   /* Updates : Edge switch params : edge_wwn, edge_fabric */

/* Used by IOC_FCR_LE_PORT_SET_STATE */
#define LE_PORT_OFFLINE 0x00  /* Mark the port online */
#define LE_PORT_ONLINE  0x01  /* Mark the port offline */
#define LE_PORT_CHANGE  0x02  /* Mark the port change */
#endif 
