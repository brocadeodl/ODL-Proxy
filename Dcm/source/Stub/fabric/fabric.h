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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Meenakshi Garg                                               *
 *   Description:                                                          *    
 *       This header file contains fabricd related stub routines           *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef FABRIC_H
#define FABRIC_H

#ifdef MAXNAMELEN
#undef MAXNAMELEN
#endif
#define MAXNAMELEN 256

#include <stdio.h>
#include <fc/wwn.h>
#include <switch/switch.h>
#include "DcmCore/DcmToolKit.h"

//#include "VcsFabric/Global/VcsFabObjectManager.h"

using namespace WaveNs;
using namespace DcmNs;

#ifdef __cplusplus
extern "C"
{
#endif

vector<UI32> getx86DomainListFromPrincipalSelectionWorker ();
vector<UI32> getx86DomainListFromFCPrincipalTrackingWorker ();

static inline int fabGetDomainList (u_short *domainList, size_t recv_size)
{

	UI32 i;
	vector<UI32> x86domainList;
	if (DcmToolKit::fcPrincipalEnabled())
	{
		x86domainList = getx86DomainListFromFCPrincipalTrackingWorker ();
	}
	else
	{
		x86domainList = getx86DomainListFromPrincipalSelectionWorker ();
	}
	domainList[0] = x86domainList.size ();
	for (i = 0; i < x86domainList.size (); i++)
	{
		domainList[i+1] = (u_short) x86domainList[i];
	}
	return 0;
}

static inline int fabGetPrincipalSwitchDomain ()
{
    return 0;
}

static inline void getDomainLst(char *domListP)
{
	int i, count = 0;
	for (i = 0; i <= 3; i++) {
		domListP[++count] = (char)i;
	}
	domListP[0] = count;
}

/* structure for passing the info. from the fabricd to client through IPC */
typedef struct fab_domain {
        int  dom_id;
        char dom_wwn[24];       /* wwn of the fabric */
        char dom_ipaddr[16];    /* IP address of the switch */
        char dom_ether[16];     /* Ethernet address of the switch */
        char dom_sep;           /* seperator for domain */
        char dom_name[36];
        char dom_ipaddrV6[48];  /* IPv6 address of the switch */
        int  sw_model;          /* switch model */
        u_int sw_fw_id;         /* firmware version */

        /*
         * Following fields are only available for domains belonging to a FCR port
         */
        char fcr_name1[32];     /* Name of the physical FCR Switch */
        char fcr_ipaddr1[16];   /* IP address of the switch */
        char fcr_ipaddr2[48];   /* Physical IPv6 address of the switch */

} fab_domain_t;

/* structure for passing the info. from the fabricd to client through IPC */
typedef struct fab_token {
        int  tok_id;            /* Token (mcast) ID */
        char tok_token[40];     /* token totally (11*3+2) bytes */
} fab_token_t;

typedef struct isl_port_info {
	int     serialport;
	int     src_port;
	int     dest_port;
	int     domainid;
	int     speed;
	int     bandwidth;
	char    domain_wwn[24];
	char    switch_name[36];
	char type[20];
} isl_port_info_t;

static inline int fabGetCnt(u_int *d_cnt, u_int *t_cnt)
{
        int rval = 0;
        *d_cnt = 5;
        return (rval);
}

static inline int getFabricIslInfo(isl_port_info_t **isl_entry_list)
{
	isl_port_info_t * isl_data;
	int i=0;
	isl_data = (isl_port_info_t *)malloc (sizeof (isl_port_info_t)* 12);
	if (isl_data != NULL) {
		memset(isl_data, 0, sizeof (isl_port_info_t) * 12);
/*		for (i = 0; i < 5 ;i++) {
				j++;
				isl_data[i].domainid = i+1;
				isl_data[i].src_port = i;
				isl_data[i].dest_port = i;
				isl_data[i].serialport = i+1;
				strcpy(isl_data[i].domain_wwn, "11:22:33:44:55:66:77:FF");
				strcpy(isl_data[i].switch_name, "switch_1");
				isl_data[i].bandwidth = i+12000;
				isl_data[i].speed = i+12000;
				strcpy(isl_data[i].type, "QOS");
			}
	*/			/* Zero WWN case */
	/*			isl_data[i].domainid = 111;
				isl_data[i].src_port = 111;
				isl_data[i].dest_port = -1;
				isl_data[i].serialport = i+1;
				strcpy(isl_data[i].domain_wwn, "00:00:00:00:00:00:00:00");
				strcpy(isl_data[i].switch_name, "switch_1");
				isl_data[i].bandwidth = i+12000;
				isl_data[i].speed = i+12000;
				strcpy(isl_data[i].type, "incompatible");
				j++;

	*/			/* NULL WWN case */
	/*			isl_data[i].src_port = 100;
				j++; */

		isl_data[i].domainid = 1;
		isl_data[i].src_port = 1;
		isl_data[i].dest_port = 1;
		isl_data[i].serialport = i+1;
		strcpy(isl_data[i].domain_wwn, "11:22:33:44:55:66:77:F1");
		strcpy(isl_data[i].switch_name, "switch_1");
		isl_data[i].bandwidth = i+2000;
		isl_data[i].speed = i+12000;
		strcpy(isl_data[i].type, "QOS");
		i++;

		isl_data[i].domainid = 200;
		isl_data[i].src_port = 200;
		isl_data[i].dest_port = 2;
		isl_data[i].serialport = i+1;
		strcpy(isl_data[i].domain_wwn, "11:22:33:44:55:66:77:F2");
		strcpy(isl_data[i].switch_name, "switch_1");
		isl_data[i].bandwidth = i+12000;
		isl_data[i].speed = i+12000;
		strcpy(isl_data[i].type, "QOS");
		i++;

		isl_data[i].domainid = 300;
		isl_data[i].src_port = 300;
		isl_data[i].dest_port = 0;
		isl_data[i].serialport = i+1;
		strcpy(isl_data[i].domain_wwn, "11:22:33:44:55:66:77:F3");
		strcpy(isl_data[i].switch_name, "switch_1");
		isl_data[i].bandwidth = i+12000;
		isl_data[i].speed = i+12000;
		strcpy(isl_data[i].type, "incompatible");
		i++;

		isl_data[i].domainid = 400;
		isl_data[i].src_port = 400;
		isl_data[i].dest_port = 402;
		isl_data[i].serialport = i+1;
		strcpy(isl_data[i].domain_wwn, "11:22:33:44:55:66:77:F4");
		strcpy(isl_data[i].switch_name, "switch_1");
		isl_data[i].bandwidth = i+12000;
		isl_data[i].speed = i+12000;
		strcpy(isl_data[i].type, "TRUNK");
		i++;

//		}
	}
	*isl_entry_list = isl_data;
	return(i); //Return entries
}
    
static inline int fabricShow(fab_domain_t *dlist, fab_token_t *tlist, int dcnt, int tcnt)
{
        int rval = 0, i = 0;

/*      for (i = 0; i < dcnt; i++) {
                dlist[i].dom_id = i+1;
                strcpy(dlist[i].dom_wwn, "11:22:33:44:55:66:77:FF");
                strcpy(dlist[i].dom_ipaddr, "192.168.38.244");
                strcpy(dlist[i].dom_ether, "192.168.38.245");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else    
                        dlist[i].dom_sep = ' ';
                strcpy(dlist[i].dom_name, "switch_1");
                strncpy(dlist[i].dom_ipaddrV6, "3ffe:1900:4545:3:200:f8ff:fe21:67cf", 48);
        }
*/
		dlist[i].dom_id = i+1;
                strcpy(dlist[i].dom_wwn, "11:22:33:44:55:66:77:FF");
                strcpy(dlist[i].dom_ipaddr, "192.168.38.244");
                strcpy(dlist[i].dom_ether, "192.168.38.245");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else
                        dlist[i].dom_sep = ' ';
                strcpy(dlist[i].dom_name, "switch_1");
                strncpy(dlist[i].dom_ipaddrV6, "3ffe:1900:4545:3:200:f8ff:fe21:67cf", 48);

                i++;

                dlist[i].dom_id = i+1;
                strcpy(dlist[i].dom_wwn, "11:22:33:44:55:66:77:dd");
//              strcpy(dlist[i].dom_ipaddr, "192.168.38.200");
//              strcpy(dlist[i].dom_ether, "192.168.38.240");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else
                        dlist[i].dom_sep = ' ';
//              strcpy(dlist[i].dom_name, "switch_2");
//              strncpy(dlist[i].dom_ipaddrV6, "fffe:1fff:4545:3:200:f8ff:fe21:67cf", 48);

                i++;

                dlist[i].dom_id = 100;
                strcpy(dlist[i].dom_wwn, "10:00:05:1e:55:66:77:FF");
                strcpy(dlist[i].dom_ipaddr, "121.118.318.244");
                strcpy(dlist[i].dom_ether, "12.168.38.245");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else
                        dlist[i].dom_sep = ' ';
                strcpy(dlist[i].dom_name, "switch_3");
                strncpy(dlist[i].dom_ipaddrV6, "none", 48);

                i++;

                dlist[i].dom_id = 101;
                strcpy(dlist[i].dom_wwn, "10:00:05:1e:55:66:77:FF");
                strcpy(dlist[i].dom_ipaddr, "121.118.318.244");
                strcpy(dlist[i].dom_ether, "12.168.38.245");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else
                        dlist[i].dom_sep = ' ';
                strcpy(dlist[i].dom_name, "switch_3");
                strncpy(dlist[i].dom_ipaddrV6, "none", 48);

		i++;

                dlist[i].dom_id = 5;
                strcpy(dlist[i].dom_wwn, "10:00:05:1e:55:66:77:FF");
                strcpy(dlist[i].dom_ipaddr, "121.118.318.244");
                strcpy(dlist[i].dom_ether, "12.168.38.245");
                if (i == 0)
                        dlist[i].dom_sep = '>';
                else
                        dlist[i].dom_sep = ' ';
                strcpy(dlist[i].dom_name, "switch_3");
                strncpy(dlist[i].dom_ipaddrV6, "none", 48);
		i++;

        return (rval);
}

static inline int fabGetSwitchShow(char *switchShowPath)
{
	FILE	*switchshow_fp = NULL;
	FILE	*tmpSwShow_fp  = NULL;
	char	fname[MAXNAMELEN];
	char	in_line[MAXNAMELEN];
	char	out_line[MAXNAMELEN];
	char	tmp_line[MAXNAMELEN];
	char	tmp_line1[MAXNAMELEN];
	char	tmp_line2[MAXNAMELEN];
	int		i = 0;
	int		log_all = 0;
	int		port_cnt = 0;
	int		sw_slot_based = 0;
	int		slot = 0, port = 0, domain = 233;
	int		slot2 = 0, port2 = 0;
	char    *tmpPtr = NULL;
	char    *token;
	char    tokendelim[] = "  ";

	snprintf(fname, 256, "/vobs/projects/springboard/fabos/src/vcs/dcm/application/switchshow.txt");

	if (0 != strncmp(switchShowPath, "/vobs/projects/springboard/fabos/src/vcs/dcm/application/switchshow", 57)) {
		return (-11);
	}

	tmpSwShow_fp = fopen(switchShowPath, "w+");
	if (!tmpSwShow_fp) {
		fclose(tmpSwShow_fp);
		return (-2);
	}

	if ((access(switchShowPath, R_OK) != 0) ||
		(access(switchShowPath, W_OK) != 0)) {
		fclose(tmpSwShow_fp);
		return (-3);
	}

	switchshow_fp = fopen(fname, "r");
	if (!switchshow_fp) {
		fclose(tmpSwShow_fp);
		return (-4);
	}

	while (!feof(switchshow_fp)) {

		if (!fgets(in_line, MAXNAMELEN, switchshow_fp)) {
			break;
		}

		/*
		 * For port related data:
		 */
		if ((strstr(in_line, "Port") != NULL) &&
			(strstr(in_line, "Index") != NULL)) {
			log_all = 1;
			if (strstr(in_line, "Slot") != NULL) {
				sw_slot_based = 1;
			}

			snprintf(out_line, sizeof (out_line), "\n %4s           %4s   %s\n",
						"Port", "State", "Operational State");
			fputs(out_line, tmpSwShow_fp);

			/*
			 * Read the next line which is "======"
			 */
			if (!fgets(in_line, MAXNAMELEN, switchshow_fp)) {
					break;
			}

			snprintf(out_line, sizeof (out_line),
						"============================================"
						"===============\n");
			fputs(out_line, tmpSwShow_fp);

			/*
			 * Read Port details
			 */
			if (!fgets(in_line, MAXNAMELEN, switchshow_fp)) {
					break;
			}
		}

		/* Begin to parse through the switchshow output line by line */
		if (log_all) {
			i = 0;

			/* Advance index to point to pertinent data and skip spaces */
			while (isspace(in_line[i]))
				i++;

			/*
			 * Need to revisit this logic
			 * on Slot based systems which is currently not suported.
			 * The below 4 is to skip ge-ports since it is the only field
			 * that has more than 4 spaces after its field.
			 */
			if (i > 4)
				continue;

			/*
			 * Read in first field which is the 'Index' field...tmpPtr will
			 * be adjusted to point after "Index" field
			 */
			slot = strtol(in_line, &tmpPtr, 10);
			
			if (sw_slot_based) {
				slot = strtol(tmpPtr, &tmpPtr, 10);
			} else {
				slot = 0;
			}
			port = strtol(tmpPtr, &tmpPtr, 10);

			/*
			 * After storing 'slot' and 'port' fields, parse through the rest
			 * of the data (i.e. 'Address', 'Media', 'Speed', 'State',
			 * 'Proto', etc).
			 */
			token = strtok(tmpPtr, tokendelim);
			i = 0;
			while (token != NULL) {

				token = strtok(NULL, tokendelim);
				if (token != NULL) {
					++i;

					/*
					 * If 'i == 3' we are at 'State' field,
					 * store field in tmp_line.
					 */
					if (i == 3) {
						if (strstr(token, "Online") != NULL) {
							strcpy(tmp_line, "Up  ");
						} else {
							strcpy(tmp_line, "Down");
						}
					} else if (i == 5) {
						/* If 'i == 5' This is the ISL description */
						if (strncmp(token, "\n", 1) == 0) {
							i--;
						} else if (strstr(token, "E-Port") != NULL) {
							/* Replace 'E-Port' with 'ISL' */
							strcat(tmp_line1, "ISL");
							snprintf(tmp_line1, sizeof (tmp_line1),
									 "ISL");
						} else if (strstr(token, "G-Port") != NULL) {
							/* Replace 'G-Port' with 'FAILED ISL' */
							snprintf(tmp_line1, sizeof (tmp_line1),
									 "ISL (protocol incomplete)");
						} else if (strstr(token, "Disabled") != NULL) {
							/*
							 * Replace "Disabled" Operational State
							 * with "Down".
							 */
							snprintf(tmp_line1, sizeof (tmp_line1),
									 "Down");
						} else {
							snprintf(tmp_line1, sizeof (tmp_line1), "%s",
										token);
						}
					} else if (i > 5) {
						/*
						 * If 'i > 5' there is nbrRbridge data,
						 * append all token fields to tmp_line1.
						 */
						if ((strstr(tmp_line1, "Down") != NULL) &&
							(strstr(token, "Persistent") != NULL)) {
							/*
							 * Replace "Disabled (Persistent)" Operational State
							 * with "Down (Admin)".
							 */
							strcat(tmp_line1, " ");
							strcat(tmp_line1, "(Admin)");
						} else if (strstr(token, "Slot") != NULL) {
							token = strtok(NULL, tokendelim);
							if (token == NULL) {
								break;
							}
							++i;

							slot2 = strtol(token, NULL, 10);
						} else if (strstr(token, "Port") != NULL) {
							token = strtok(NULL, tokendelim);
							if (token == NULL) {
								break;
							}
							++i;

							port2 = strtol(token, NULL, 10);
							snprintf(tmp_line2, sizeof (tmp_line2),
									 "Te %d/%d/%d", domain, slot2, port2);
							strcat(tmp_line1, " ");
							strcat(tmp_line1, tmp_line2);
						} else {
							strcat(tmp_line1, " ");
							strcat(tmp_line1, token);
						}
					}
				}
			}

			/*
			 * If there is no nbrRbridge description,
			 * just store domain, slot, port, and tmp_line
			 * ('State' field) to out_line.
			 */
			if (i < 5) {
				if (strstr(tmp_line, "\n") != NULL) {
					snprintf(out_line, sizeof (out_line), "Te %d/%d/%-3d\t%s",
							domain, slot, port, tmp_line);
				} else {
					snprintf(out_line, sizeof (out_line), "Te %d/%d/%-3d\t%s\n",
							domain, slot, port, tmp_line);
				}
			} else {
				/*
				 * If there is nbrRbridge description,
				 * store domain, slot, port, tmp_line ('State' field)
				 * and nbrRbridge description (tmp_line1) to out_line.
				 */
				if (strstr(tmp_line1, "\n") != NULL) {
					snprintf(out_line, sizeof (out_line), "Te %d/%d/%-3d\t%-4s"
														  "    %s",
							domain, slot, port, tmp_line, tmp_line1);
				} else {
					snprintf(out_line, sizeof (out_line), "Te %d/%d/%-3d\t%-4s"
														  "    %s\n",
							domain, slot, port, tmp_line, tmp_line1);
				}
			}


			/*
			 * We don't want to display F-Port info
			 * as part of this.
			 */
			if ((i >= 3) && strstr(out_line, "F-Port") == NULL) {
				fputs(out_line, tmpSwShow_fp);
			}
			++port_cnt;
			continue;
		}

		/*
		 * Include these Switch details:
		 */
		if ((strstr(in_line, "Name:")	!= NULL) ||
			(strstr(in_line, "Type:")	!= NULL) ||
			(strstr(in_line, "State:")	!= NULL) ||
			(strstr(in_line, "Role:")	!= NULL) ||
			(strstr(in_line, "RBridge-ID:")	!= NULL) ||
			(strstr(in_line, "WWN:")	!= NULL)) {
			fputs(in_line, tmpSwShow_fp);
		}

		/*
		 * Ignore the rest of the switchShow output.
		 */
	}

	fclose(tmpSwShow_fp);
	fclose(switchshow_fp);

	return (0);

}

static inline int fabPortLogShow(char *portLogShowPath)
{
	return (0);
}

static inline int fabPortLogClear()
{
	return (0);
}

typedef struct trunk_port_info {
    int     group_num;
    int     src_port_num;
    int     dest_port_num;
    int     deskew;
    int     domain_id;
    int     type;
	bool 	port_type;
    char switch_wwn[24];
    unsigned long int     tx_bandwidth;
    unsigned long int     tx_throughput;
    unsigned long int     tx_percent;
    unsigned long int     rx_bandwidth;
    unsigned long int     rx_throughput;
    unsigned long int     rx_percent;
} trunk_entry_t;

static inline int getFabricTrunkInfo(trunk_entry_t **trunk_entry_list)
{
    trunk_entry_t *trunk_data;
    int i = 0, j = 0;
    int domain=3;

    trunk_data = (trunk_entry_t *)malloc (sizeof (trunk_entry_t) * 10);
    if (trunk_data != NULL) {
		memset(trunk_data, 0, sizeof (trunk_entry_t) * 10);
		for (j = 0; j < 5 ;j++ , i++) {
			trunk_data[j].group_num=1;
			trunk_data[j].src_port_num=i + 100;
			trunk_data[j].dest_port_num=i + 200;
			strcpy(trunk_data[j].switch_wwn, "10:00:01:02:03:04:05:88");
			trunk_data[j].deskew=12;
			trunk_data[j].domain_id=domain;
			trunk_data[j].type=i;
            
			trunk_data[j].tx_bandwidth=12;
			trunk_data[j].tx_throughput=896;
			trunk_data[j].tx_percent=0;
			trunk_data[j].rx_bandwidth=12;
			trunk_data[j].rx_throughput=896;
			trunk_data[j].rx_percent=0;
	}
		trunk_data[0].src_port_num=1;
		trunk_data[0].dest_port_num=2;
		trunk_data[0].domain_id=1;
		trunk_data[4].group_num=10;

    }
    *trunk_entry_list = trunk_data;
    return(j); //Return entries
}

static inline int fabSetLocalDomainId(int domainId)
{
	printf("Local DomainId: %d\n", domainId);
	return (domainId);
}

static inline int fabSetInsistentDomainIdMode(int ididMode)
{
	printf("Insistent DomainId: %s\n", (ididMode ? "enabled" : "disabled"));
	return (ididMode);
}

static inline int fabGetIslList(wwn_t *nbr_wwn, int *portList, size_t sz)
{
	portList[0] = 2;
	portList[1] = 1;
	portList[2] = 2;

	return (0);
}

static inline int fabToggleWWNLinks(wwn_t *wwn)
{
    return (0);
}

static inline int fabGetWwn(int domain, wwn_t *wwn)
{
    return (1);
}

static inline  u_int fabGetModel(int)
{
	return (1);
}

static inline int fabPrinSelModeCmd (int modeCmd, int priority)
{
    return (1);
}

#ifdef __cplusplus
}
#endif

#endif // FABRIC_H
