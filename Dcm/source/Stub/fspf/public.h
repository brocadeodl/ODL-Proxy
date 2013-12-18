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

#ifndef __FSPFPUBLIC_H__
#define __FSPFPUBLIC_H__

#include "fc/fspf.h"
#include "fspf/ipc.h"
#include "sys/fc/fspf.h"
#include <fc/wwn.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Place all #defines here */
#define FSPF_MAX_DOMAINS 240
#define MAX_ISL 5

/*  
 * Error return values for getTopologyInfo()
 */
enum {
        ERR_NO_FABRIC           =       -1,
        ERR_DOMAIN_UNREACHABLE  =       -2,
        ERR_DOMAIN_INVALID      =       -3,
        ERR_IPC_FAILED          =       -4,
        ERR_MALLOC_FAILED       =       -5      
};

/* Place all data structure definitions here */
typedef struct bcast_entry {
	int port_isl; /* ISL ports */
	int bcast_route_avail; /* bcast route present for this port*/
	int localPort;
	int nbrDomain;
	int nbrPort;
	int trunkPort;
	int bandwidth;
} bcast_entry_t;

typedef struct bcast_info {
	int portCount;				/* number of ISL ports */
	int bcastRootDomain;		/* root of mcast/bcast tree */
	int bcastRootPriority;		/* mcast/bcast root priority */
	u_int bcastRootIPAddr;		/* bcast root domain's IPAddr */
	char bcastRootWwn[24];		/* bcast root domain's WWN */
	char bcastRootSwname[32];	/* bcast root domain's Switchname */
	int bcastLocalPriority;		/* local domain's priority */
	bcast_entry_t *bcast_entry_list;
} bcast_info_t;

/* Topology structure */
typedef struct topo_path_entry {
	pdb_t path_entry;
	int	nbrDomain;
	int	nbrPort;
	int	trunkPort;
} topo_path_entry_t;

typedef struct topology {
	topo_ipc_t domain_info;
	topo_path_entry_t path_info[MAX_PATHS];
} topology_t;

static inline int getIslCnt(void)
{
	return (MAX_ISL);
}

static inline void getBcastInfo(bcast_info_t *bcast_info_bufP, int index)
{
	bcast_entry_t *isl_list;
	int i;
	int isl_cnt = getIslCnt();

	isl_list = (bcast_entry_t *)malloc(sizeof (bcast_entry_t) * 5);

	if (isl_list != NULL) {
		memset((char *)isl_list, 0, sizeof (bcast_entry_t) * isl_cnt);
		for (i = 0; i < isl_cnt; i++) {
			isl_list[i].port_isl = i;
			isl_list[i].bcast_route_avail = 1;
			isl_list[i].localPort = i;
			isl_list[i].nbrDomain = i;
			isl_list[i].nbrPort = i;
			isl_list[i].trunkPort = (i % 2);
			isl_list[i].bandwidth = i * 1000;
		}
		isl_list[4].localPort = 333;
		isl_list[4].nbrDomain = 133;
		isl_list[4].nbrPort = 222;
		isl_list[4].bandwidth = 10000;

		bcast_info_bufP->bcast_entry_list = isl_list;
	}

	bcast_info_bufP->portCount = 5;
	bcast_info_bufP->bcastRootDomain = 33;
	bcast_info_bufP->bcastRootPriority = 1;
	bcast_info_bufP->bcastRootIPAddr = 0x11223344;
	strcpy(bcast_info_bufP->bcastRootWwn,"11:22:33:44:55:66:77:88");
	strcpy(bcast_info_bufP->bcastRootSwname,"RBridgeStub");
	bcast_info_bufP->bcastLocalPriority = 10;
}

static inline int domainReachable(int domain) {
	return (1);
}
/* topologyShow routines */
static inline int getTopologyInfo(int domain, topology_t *topo_entry_list)
{

	int dom_count = domain ? 1 : 3;
	int i, j;

	for (i = 0; i < dom_count; i++) {
		topo_entry_list[i].domain_info.t_domain = domain ? domain : i + 1;
		topo_entry_list[i].domain_info.t_metric = 500;
		strcpy(topo_entry_list[i].domain_info.t_name,"switch1");
		topo_entry_list[i].domain_info.t_uHopsFromRoot = 1;
		topo_entry_list[i].domain_info.t_bps = 100;
		topo_entry_list[i].domain_info.t_pathCnt = i + 2;

		for (j = 0; j < topo_entry_list[i].domain_info.t_pathCnt; j++) {
			topo_entry_list[i].path_info[j].path_entry.pflags = (i % 2) ? F_STATIC : 0;
			topo_entry_list[i].path_info[j].path_entry.portcnt = j + 1;
			topo_entry_list[i].path_info[j].path_entry.metric = 500;
			topo_entry_list[i].path_info[j].path_entry.outport = 5;
			topo_entry_list[i].path_info[j].path_entry.curr_bw_alloc = 800;	
			topo_entry_list[i].path_info[j].path_entry.bw_utilization = 10;	
			topo_entry_list[i].path_info[j].path_entry.total_bw = 800;
			topo_entry_list[i].path_info[j].nbrDomain = j + 2;
			topo_entry_list[i].path_info[j].nbrPort = j + 1;
			topo_entry_list[i].path_info[j].trunkPort = i % 2;
		}
	}

	if (dom_count == 3) {
		topo_entry_list[2].domain_info.t_domain = 200;
		topo_entry_list[2].domain_info.t_metric = 1000;
		strcpy(topo_entry_list[2].domain_info.t_name,"switch1");
		topo_entry_list[2].domain_info.t_uHopsFromRoot = 10;
	
		topo_entry_list[2].path_info[0].path_entry.metric = 1000;
		topo_entry_list[2].path_info[0].path_entry.outport = 500;
		topo_entry_list[2].path_info[0].path_entry.curr_bw_alloc = 800;	
		topo_entry_list[2].path_info[0].path_entry.bw_utilization = 10;	
		topo_entry_list[2].path_info[0].path_entry.total_bw = 10000;
		topo_entry_list[2].path_info[0].nbrDomain = 233;
		topo_entry_list[2].path_info[0].nbrPort = 133;
		topo_entry_list[2].path_info[0].trunkPort = 1;
	}

	return (dom_count);
}

/* BcastPriority */
static inline int setMcastPriority(int priority)
{
	return (0);
}

/*
 * The ls_info and links_info structures commented here
 * is includedd in /sys/fc/fspf.h
 */
#if 0
/*
 * Link State info.
 */
typedef struct ls_info {
	int linkId;		/* Link ID */
	int outPort;	/* Output port for this link */
	int remPort;	/* Remote port */
	u_short cost;   /* Cost of sending a frame over this ISL */
	u_char  costCnt;/* Number of Class costs */
	u_char  type;   /* Link type */
	u_char	dispType;	/* Port Display type */
	u_char	blade;	/* Blade ID */
	u_char	phyPort;	/* External interface number */
	u_short	reserved1;	/* Reserved for later use */
	u_short flags:15;	/* Reserved for fugure flags */
	u_short trunked:1;	/* Indicates if outPort is T-Port */
	int reserved2;		/* Reserved for future use */
	int	reserved3;		/* Reserved for future use */
} lsi_t;
typedef struct links_info {
	int domainid;
	int domainReachable;
	int version;
	char domain_wwn[24];
	int numberOfLinks;
	lsi_t *links;
} links_info_t;
#endif 

static inline int
getFabricLinkInfo(links_info_t **isl_list)
{
	links_info_t *isl_data;
	unsigned int domain=0;
	unsigned int  maxdomains = 2;
	int num_link = 0;
	isl_data = (links_info_t *)malloc (sizeof (links_info_t)* 5);
	memset(isl_data, 0, sizeof (links_info_t) * 5);
	if (isl_data != NULL) {
			for (domain = 0; domain < maxdomains; domain++) {
					isl_data[domain].domainid = domain + 1;
					isl_data[domain].domainReachable = 1;
					isl_data[domain].version = 1;
					isl_data[domain].numberOfLinks = 2;
					strcpy(isl_data[domain].domain_wwn, "10:00:00:05:1e:40:56:00");
					isl_data[domain].links = (lsi_t *)malloc(2 * sizeof (lsi_t));
					memset(isl_data[domain].links, 0, 2 * sizeof (lsi_t));

					/* Number of Links per Domain */
					for (num_link = 0; num_link < 2; num_link++) {
							isl_data[domain].links[num_link].linkId = domain + 20;
							isl_data[domain].links[num_link].outPort = domain + 12;
							isl_data[domain].links[num_link].remPort = domain + 13;
							isl_data[domain].links[num_link].type = 0;
							isl_data[domain].links[num_link].costCnt = 1;
							isl_data[domain].links[num_link].cost = domain + 201;
							isl_data[domain].links[num_link].dispType = 1;
							isl_data[domain].links[num_link].blade = 0;
							isl_data[domain].links[num_link].phyPort = domain + 14;
							isl_data[domain].links[num_link].trunked = num_link % 2;
					}
			}
	}
	*isl_list = isl_data;
	return(maxdomains);
}
#define FSPF_MAX_LINKS 2048    /* Fake max port # for sanity check */

#define OK 0

typedef struct isl_info {
    int localPort;
    int domain;
    int remPort;
    int fspfState;
    int linkCost;
    char    nbrState[32];
} isl_info_t;

static inline int
getIslInfo(int ifNo, isl_info_t *islListP)
{
	return (0);
}

static inline int
getIslList(isl_info_t *islListP)
{
	return (0);
}

#ifdef __cplusplus
}
#endif

#endif // __FSPFPUBLIC_H__
