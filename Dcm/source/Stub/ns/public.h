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

#ifndef __NSPUBLIC_H__
#define __NSPUBLIC_H__

#include "fabric/getid.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NSDB_GETEXACT 1
#define NS_FCPINQ_DEVINFO_SIZE  28

typedef wwn_t       nswwn_t;

typedef struct nssymb {
    char    ns_symb[256];
} nssymb_t;

typedef char   nsipaddr_t[16];

typedef unsigned char   nsipa_t[8];

typedef unsigned long   nscos_t;

typedef u_long  nsfc4_t[8];

typedef struct ns_entry {
    u_char      ns_ptype;   /* port type */
    unsigned    ns_pid:24;  /* port ID of the entry */

    nswwn_t     ns_pname;   /* portname */
    nssymb_t    ns_psymb;   /* port symbolic name */

    nswwn_t     ns_nname;   /* node name */
    nssymb_t    ns_nsymb;   /* node symbolic name */

    nsipa_t     ns_ipa;     /* initial process associator */
    nsipaddr_t  ns_ipaddr;  /* IP address */
    nscos_t     ns_cos;     /* class of service */
    nsfc4_t     ns_fc4s;    /* FC-4 types */

    /* added to support FC-GS2, Rev 5.3; see also ns_entryOld_t */
    nsipaddr_t  ns_ipport;  /* IP address of the Nx_Port */
    nswwn_t     ns_fxpname; /* WWN of the Fx_Port */
    u_char      ns_devtype; /* device type */
    unsigned    ns_hardaddr:24; /* hard address */
    u_int       ns_portindex; /* port index */
    u_int       ns_sharearea; /* shared area flag */

} ns_entry_t;

typedef struct ns_detail_entry {
	ns_entry_t 	ns_med;

	// additional information on top of ns_entry_t
    int 		ns_scr;     						/* SCR registration data */
	char 		ns_fcp_inq[NS_FCPINQ_DEVINFO_SIZE]; /* FCP inquiry data */
	nswwn_t 	ns_ppn;     						/* Permanent port name */
    int 		is_redirect;
    int 		is_vivt;
    int 		is_rdhost;
    int 		is_rdtarget;
    int 		is_partial;
	u_int		is_connected_via_ag:1;				/* Access Gateway */
	u_int		ns_real:1;							/* this bit is set if real
													   device behind an AG */
	u_int		ns_cascaded:1;						/* this bit is set if its a
													   cascaded AG */
	u_int		is_ag_base_device:1; 				/* Indicates if the device 
													   is a Base AG Device */
	u_int		reserved0:28;
} ns_detail_entry_t;

#define isZero(n)   ((n) == 0)
#define TRUE 1
#define SYMB_STR	"SANBlaze V5.5-BRCDrc11 Port"
#define SYMB_STR_N	"SANBlaze V5.5-BRCDrc11 Port Node"
#define NsGetSymbLen(p) (p)->ns_symb[0]
#define ROUND_UP(x, align)  (((int) (x) + (align - 1)) & ~(align - 1))
#define NSMEMCPY(d, s, l)   memcpy((void *)(d), (void *)(s), (l))

static inline char *
nsSymbToStrEx(nssymb_t *s, char *ns_xbuf, uint size)
{
    int     i, len, slen;
    int     printable = TRUE;
    char        lbuf[4];
    char        tmp[256];

    slen = len = NsGetSymbLen(s);
    if (isZero(len)) {
        strcpy(ns_xbuf, "NULL");
    } else {
        len = ROUND_UP(len+1, sizeof (int));
        NSMEMCPY(tmp, s->ns_symb, len);
        ns_xbuf[0] = 0;
        sprintf(ns_xbuf, "[%d] 0x", slen);
        for (i = 1; i <= slen; i++) {
            printable = printable && (isprint(tmp[i]));
            sprintf(lbuf, "%02x", tmp[i]);
            strcat(ns_xbuf, lbuf);
        }
        if (printable) {
            sprintf(ns_xbuf, "[%d] \"", slen);
            i = strlen(ns_xbuf);
            NSMEMCPY(&ns_xbuf[i], &tmp[1], slen);
            i += slen;
            ns_xbuf[i++] = '"';
            ns_xbuf[i] = 0;
        }
    }
    return (ns_xbuf);
}

static inline int nsBulkWwnToIds_full_db(int count,
	wwn_t *wwn_list, u_int **pid_list) {

	u_int *tmp_list;

	if (count != 1) {
	    return (0);
	}

	if (wwn_list[0].chars[3] == 0x07 && wwn_list[0].chars[0] == 0x22) {
	    if ((tmp_list = (u_int *)malloc (sizeof (u_int)))) {
	        tmp_list[0] = 0x070100;
	        *pid_list = tmp_list;
	        return (1);
	    } else {
	        return (0);
	    }
	} else if (wwn_list[0].chars[3] == 0x07 && wwn_list[0].chars[0] == 0x11) {
	    if ((tmp_list = (u_int *)malloc ((sizeof (u_int) * 2)))) {
	        tmp_list[0] = 0x070100;
	        tmp_list[1] = 0x090100;
	        *pid_list = tmp_list;
	        return (2);
	    } else {
	        return (0);
	    }
	} else {
	    return (0);
	}
}

static inline int fillinDetailEntry(
	u_int pid, u_int fs4s, int port_type, int port_index,
	ns_detail_entry_t *ns_detail_entry)
{
	ns_detail_entry->ns_med.ns_pid = pid;
	ns_detail_entry->ns_med.ns_fc4s[0] = fs4s;
	ns_detail_entry->ns_med.ns_fc4s[1] = 0;
	ns_detail_entry->ns_med.ns_fc4s[2] = 0;
	ns_detail_entry->ns_med.ns_fc4s[3] = 0;
	ns_detail_entry->ns_med.ns_fc4s[4] = 0;
	ns_detail_entry->ns_med.ns_fc4s[5] = 0;
	ns_detail_entry->ns_med.ns_fc4s[6] = 0;
	ns_detail_entry->ns_med.ns_fc4s[7] = 0;
	ns_detail_entry->ns_med.ns_ptype = port_type;
	ns_detail_entry->ns_med.ns_cos = NSCOS_3 | NSCOS_F;
	ns_detail_entry->ns_med.ns_psymb.ns_symb[0] = strlen(SYMB_STR);
	strcpy(&ns_detail_entry->ns_med.ns_psymb.ns_symb[1], SYMB_STR);
	ns_detail_entry->ns_med.ns_nsymb.ns_symb[0] = strlen(SYMB_STR_N);
	strcpy(&ns_detail_entry->ns_med.ns_nsymb.ns_symb[1], SYMB_STR_N);
	ns_detail_entry->ns_med.ns_portindex = port_index;
	ns_detail_entry->ns_med.ns_devtype =
	    NS_DEV_VIRTUAL << NS_DEV_PHY_VIR_SHIFT | NS_DEV_HOST;
	ns_detail_entry->ns_med.ns_pname.chars[0] = (pid & 0xff0000) >> 16;
	ns_detail_entry->ns_med.ns_pname.chars[1] = (pid & 0x00ff00) >> 8;
	ns_detail_entry->ns_med.ns_pname.chars[2] = (pid & 0x0000ff);
	ns_detail_entry->ns_med.ns_pname.chars[3] = 0x11;
	ns_detail_entry->ns_med.ns_pname.chars[4] = 0x22;
	ns_detail_entry->ns_med.ns_pname.chars[5] = 0x33;
	ns_detail_entry->ns_med.ns_pname.chars[6] = 0x44;
	ns_detail_entry->ns_med.ns_pname.chars[7] = 0x55;

	ns_detail_entry->ns_med.ns_nname.chars[0] = (pid & 0xff0000) >> 16;
	ns_detail_entry->ns_med.ns_nname.chars[1] = (pid & 0x00ff00) >> 8;
	ns_detail_entry->ns_med.ns_nname.chars[2] = (pid & 0x0000ff);
	ns_detail_entry->ns_med.ns_nname.chars[3] = 0x22;
	ns_detail_entry->ns_med.ns_nname.chars[4] = 0x33;
	ns_detail_entry->ns_med.ns_nname.chars[5] = 0x44;
	ns_detail_entry->ns_med.ns_nname.chars[6] = 0x55;
	ns_detail_entry->ns_med.ns_nname.chars[7] = 0x66;

	ns_detail_entry->ns_med.ns_fxpname.chars[0] = (pid & 0xff0000) >> 16;
	ns_detail_entry->ns_med.ns_fxpname.chars[1] = (pid & 0x00ff00) >> 8; 
	ns_detail_entry->ns_med.ns_fxpname.chars[2] = (pid & 0x0000ff);
	ns_detail_entry->ns_med.ns_fxpname.chars[3] = 0x33;
	ns_detail_entry->ns_med.ns_fxpname.chars[4] = 0x44;
	ns_detail_entry->ns_med.ns_fxpname.chars[5] = 0x55;
	ns_detail_entry->ns_med.ns_fxpname.chars[6] = 0x66;
	ns_detail_entry->ns_med.ns_fxpname.chars[7] = 0x77;

	strcpy(ns_detail_entry->ns_med.ns_ipaddr, "10.10.0.1");
	ns_detail_entry->ns_med.ns_ipa[0] = 1;
	ns_detail_entry->ns_med.ns_ipa[1] = 1;
	ns_detail_entry->ns_med.ns_ipa[2] = 1;
	ns_detail_entry->ns_med.ns_ipa[3] = 1;
	ns_detail_entry->ns_med.ns_ipa[4] = 1;
	ns_detail_entry->ns_med.ns_ipa[5] = 1;
	ns_detail_entry->ns_med.ns_ipa[6] = 1;
	ns_detail_entry->ns_med.ns_ipa[7] = 1;

	ns_detail_entry->ns_scr = 1;
	ns_detail_entry->ns_fcp_inq[0] = strlen ("local_inq");
	strcpy(&ns_detail_entry->ns_fcp_inq[1], "local_inq");

	ns_detail_entry->ns_ppn.chars[0] = (pid & 0xff0000) >> 16;
	ns_detail_entry->ns_ppn.chars[1] = (pid & 0x00ff00) >> 8;
	ns_detail_entry->ns_ppn.chars[2] = (pid & 0x0000ff);
	ns_detail_entry->ns_ppn.chars[3] = 0x44;
	ns_detail_entry->ns_ppn.chars[4] = 0x55;
	ns_detail_entry->ns_ppn.chars[5] = 0x66;
	ns_detail_entry->ns_ppn.chars[6] = 0x77;
	ns_detail_entry->ns_ppn.chars[7] = 0x78;

	ns_detail_entry->is_redirect = 1;
	ns_detail_entry->is_vivt = 1;
	ns_detail_entry->is_rdhost = 0;
	ns_detail_entry->is_rdtarget = 0;

	strcpy(ns_detail_entry->ns_med.ns_ipport, "10.10.0.2");

	return (0);
}

static inline int fillinEntry(
	u_int pid, u_int fs4s, int port_type, int port_index,
	ns_entry_t *ns_entry)
{
	ns_entry->ns_pid = pid;
	ns_entry->ns_fc4s[0] = fs4s;
	ns_entry->ns_fc4s[1] = 0;
	ns_entry->ns_fc4s[2] = 0;
	ns_entry->ns_fc4s[3] = 0;
	ns_entry->ns_fc4s[4] = 0;
	ns_entry->ns_fc4s[5] = 0;
	ns_entry->ns_fc4s[6] = 0;
	ns_entry->ns_fc4s[7] = 0;
	ns_entry->ns_ptype = port_type;
	ns_entry->ns_cos = NSCOS_3 | NSCOS_F;
	ns_entry->ns_psymb.ns_symb[0] = strlen(SYMB_STR);
	strcpy(&ns_entry->ns_psymb.ns_symb[1], SYMB_STR);
	ns_entry->ns_nsymb.ns_symb[0] = strlen(SYMB_STR_N);
	strcpy(&ns_entry->ns_nsymb.ns_symb[1], SYMB_STR_N);
	ns_entry->ns_portindex = port_index;
	ns_entry->ns_devtype =
	    NS_DEV_VIRTUAL << NS_DEV_PHY_VIR_SHIFT | NS_DEV_HOST;
	ns_entry->ns_pname.chars[0] = (pid & 0xff0000) >> 16;
	ns_entry->ns_pname.chars[1] = (pid & 0x00ff00) >> 8;
	ns_entry->ns_pname.chars[2] = (pid & 0x0000ff);
	ns_entry->ns_pname.chars[3] = 0x11;
	ns_entry->ns_pname.chars[4] = 0x22;
	ns_entry->ns_pname.chars[5] = 0x33;
	ns_entry->ns_pname.chars[6] = 0x44;
	ns_entry->ns_pname.chars[7] = 0x55;

	ns_entry->ns_nname.chars[0] = (pid & 0xff0000) >> 16;
	ns_entry->ns_nname.chars[1] = (pid & 0x00ff00) >> 8;
	ns_entry->ns_nname.chars[2] = (pid & 0x0000ff);
	ns_entry->ns_nname.chars[3] = 0x22;
	ns_entry->ns_nname.chars[4] = 0x33;
	ns_entry->ns_nname.chars[5] = 0x44;
	ns_entry->ns_nname.chars[6] = 0x55;
	ns_entry->ns_nname.chars[7] = 0x66;

	ns_entry->ns_fxpname.chars[0] = (pid & 0xff0000) >> 16;
	ns_entry->ns_fxpname.chars[1] = (pid & 0x00ff00) >> 8; 
	ns_entry->ns_fxpname.chars[2] = (pid & 0x0000ff);
	ns_entry->ns_fxpname.chars[3] = 0x33;
	ns_entry->ns_fxpname.chars[4] = 0x44;
	ns_entry->ns_fxpname.chars[5] = 0x55;
	ns_entry->ns_fxpname.chars[6] = 0x66;
	ns_entry->ns_fxpname.chars[7] = 0x77;

	strcpy(ns_entry->ns_ipaddr, "10.10.0.1");
	ns_entry->ns_ipa[0] = 1;
	ns_entry->ns_ipa[1] = 1;
	ns_entry->ns_ipa[2] = 1;
	ns_entry->ns_ipa[3] = 1;
	ns_entry->ns_ipa[4] = 1;
	ns_entry->ns_ipa[5] = 1;
	ns_entry->ns_ipa[6] = 1;
	ns_entry->ns_ipa[7] = 1;

	strcpy(ns_entry->ns_ipport, "10.10.0.2");

	return (0);
}

static inline int nsGetLocalDetailEntryByPid(u_int pid,
	ns_detail_entry_t *ns_detail_entry)
{
	if (pid == 0x070100) {
	    memset ((char *)ns_detail_entry, 0, sizeof (ns_detail_entry_t));
	    fillinDetailEntry(0x070100, 32, 1, 25, ns_detail_entry);
	    return (0);
	} else if (pid == 0x090100) {
	    memset ((char *)ns_detail_entry, 0, sizeof (ns_detail_entry_t));
	    fillinDetailEntry(0x090100, 32, 1, 26, ns_detail_entry);
	    return (0);
	} else {
	    return (-1);
	}
}


static inline int nsGetEntryByPid_no_ad(u_int pid,
	int gettype, ns_entry_t *ns_entry)
{
	if (pid == 0x070100) {
	    memset ((char *)ns_entry, 0, sizeof (ns_entry_t));
	    fillinEntry(0x070100, 32, 1, 25, ns_entry);
	    return (0);
	} else if (pid == 0x090100) {
	    memset ((char *)ns_entry, 0, sizeof (ns_entry_t));
	    fillinEntry(0x090100, 32, 1, 26, ns_entry);
	    return (0);
	} else {
	    return (-1);
	}
}

static inline int nsGetDetailEntryByPid_no_ad(u_int pid,
	int gettype, ns_detail_entry_t *ns_entry)
{
	if (pid == 0x070100) {
	    memset ((char *)ns_entry, 0, sizeof (ns_entry_t));
	    fillinEntry(0x070100, 32, 1, 25, &ns_entry->ns_med);
	    return (0);
	} else if (pid == 0x090100) {
	    memset ((char *)ns_entry, 0, sizeof (ns_entry_t));
	    fillinEntry(0x090100, 32, 1, 26, &ns_entry->ns_med);
	    return (0);
	} else {
	    return (-1);
	}
}

#ifdef __cplusplus
}
#endif

using namespace WaveNs;

namespace DcmNs
{


static inline char * nsIpAddrToStr(u_char *ip, char *buf, uint size)
{

		sprintf(buf, "%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d",
		(ip[0] & 0xFF),	(ip[1] & 0xFF),
		(ip[2] & 0xFF),	(ip[3] & 0xFF),

		(ip[4] & 0xFF),	(ip[5] & 0xFF),
		(ip[6] & 0xFF),	(ip[7] & 0xFF),

		(ip[8] & 0xFF), (ip[9] & 0xFF),
		(ip[10] & 0xFF), (ip[11] & 0xFF),

		(ip[12] & 0xFF), (ip[13] & 0xFF),
		(ip[14] & 0xFF), (ip[15] & 0xFF));

	return (buf);
}

static inline int nsGetLocalDetailEntries(ns_detail_entry_t **ns_detail_entry_list)
{
#define	DETAIL_RET_ENTRIES	3
	ns_detail_entry_t *local_list;
	int i;

	local_list = (ns_detail_entry_t *)malloc (sizeof(ns_detail_entry_t) *
	    DETAIL_RET_ENTRIES);

	if (local_list != NULL) {
	    memset ((char *)local_list, 0, sizeof (ns_detail_entry_t) *
	        DETAIL_RET_ENTRIES);

	    for (i = 0; i < DETAIL_RET_ENTRIES; i++) {
	        fillinDetailEntry(getMyDomain() << 16 | i << 8,
	            ((i % 3) == 0) ? 32 : ((i % 3) == 1) ? 256 : 0,
	            ((i % 3) == 0) ? 1 : ((i % 3) == 1) ? 2 : 0x7f,
	            i + 25, &local_list[i]);
	    }

	    *ns_detail_entry_list = local_list;
	    return (DETAIL_RET_ENTRIES);
	} else {
	    return (0);
	}
}

static inline int nsGetAllDetailEntries_fullDb(ns_detail_entry_t **ns_detail_entry_list)
{
#define	DETAIL_RET_ENTRIES	3
	ns_detail_entry_t *local_list;
	int i;

	local_list = (ns_detail_entry_t *)malloc (sizeof(ns_detail_entry_t) *
	    DETAIL_RET_ENTRIES);

	if (local_list != NULL) {
	    memset ((char *)local_list, 0, sizeof (ns_detail_entry_t) *
	        DETAIL_RET_ENTRIES);

	    for (i = 0; i < DETAIL_RET_ENTRIES; i++) {
	        fillinDetailEntry(getMyDomain() << 16 | i << 8,
	            ((i % 3) == 0) ? 32 : ((i % 3) == 1) ? 256 : 0,
	            ((i % 3) == 0) ? 1 : ((i % 3) == 1) ? 2 : 0x7f,
	            i + 25, &local_list[i]);
	    }

	    *ns_detail_entry_list = local_list;
	    return (DETAIL_RET_ENTRIES);
	} else {
	    return (0);
	}
}

static inline int nsGetAllEntries_full_db(ns_entry_t **ns_entry_list)
{
#define	DETAIL_RET_ENTRIES_9	9
	ns_entry_t *local_list;
	int i;
	int domain_id;

	local_list = (ns_entry_t *)malloc (sizeof(ns_entry_t) *
	    DETAIL_RET_ENTRIES_9);

	if (local_list != NULL) {
	    memset ((char *)local_list, 0, sizeof (ns_entry_t) *
	        DETAIL_RET_ENTRIES_9);

	    for (i = 0; i < DETAIL_RET_ENTRIES_9; i++) {
	        if ((i % 3) == 0) {
	            domain_id = getMyDomain();
	        } else if ((i % 3) == 1) {
	            domain_id = getMyDomain() + 1;
	        } else {
	            domain_id = getMyDomain() + 2;
	        }
	   
	        fillinEntry(domain_id << 16 | i << 8,
	            ((i % 3) == 0) ? 32 : ((i % 3) == 1) ? 256 : 0,
	            ((i % 3) == 0) ? 1 : ((i % 3) == 1) ? 2 : 0x7f,
	            i + 25, &local_list[i]);
	    }

	    *ns_entry_list = local_list;
	    return (DETAIL_RET_ENTRIES_9);
	} else {
	    return (0);
	}
}

static inline int nsGetZonedEntryByPid(u_int pid, ns_entry_t **ns_entry_list)
{
#define	DETAIL_RET_ENTRIES_9	9
	ns_entry_t *local_list;
	int i;
	int domain_id;

	local_list = (ns_entry_t *)malloc (sizeof(ns_entry_t) *
	    DETAIL_RET_ENTRIES_9);

	if (local_list != NULL) {
	    memset ((char *)local_list, 0, sizeof (ns_entry_t) *
	        DETAIL_RET_ENTRIES_9);

	    for (i = 0; i < DETAIL_RET_ENTRIES_9; i++) {
	        if ((i % 3) == 0) {
	            domain_id = getMyDomain();
	        } else if ((i % 3) == 1) {
	            domain_id = getMyDomain() + 1;
	        } else {
	            domain_id = getMyDomain() + 2;
	        }
	   
	        fillinEntry(domain_id << 16 | i << 8,
	            ((i % 3) == 0) ? 32 : ((i % 3) == 1) ? 256 : 0,
	            ((i % 3) == 0) ? 1 : ((i % 3) == 1) ? 2 : 0x7f,
	            i + 25, &local_list[i]);
	    }

	    *ns_entry_list = local_list;
	    return (DETAIL_RET_ENTRIES_9);
	} else {
	    return (0);
	}
}
}

#endif // __NSPUBLIC_H__
