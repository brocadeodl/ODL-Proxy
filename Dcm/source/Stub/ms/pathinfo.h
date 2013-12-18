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
 *   Author : Ravikumar S                                                  *
 *   Description:                                                          *    
 *       This header file contains msd related stub routines               *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef PATHINFO_H
#define PATHINFO_H

#define PATHI_DEF_HOPS  25
#define	STRING_64_LEN	24		/* Min length for 64-bit string */
/*
 * Path Info Request
 */
typedef struct {
	u_int	ls_code;	/* code ELS_PATHI */
#if __BYTE_ORDER == __BIG_ENDIAN
	u_int	version:8;	/* pathinfo protocol version */
	u_int	flags:24;	/* Flags */
	short	length;		/* Sequence total length */
	u_char	pType;		/* Port type */
	char	error;		/* Error code */
	u_int	d_id:24;	/* ID of the path destination's port */
	u_int	hdr_len:8;	/* Header length */
	u_int	s_id:24;	/* ID of the path source's port */
	int	hops:8;		/* Residual hops */
	u_short msgId;		/* pathinfo query ID */
	u_short	rsvd1;		/* Reserved */
	short   srcRt;		/* Source Route pointer (offset) */
	short	rsvd2;		/* Reserved */
	short 	sfId;	        /* source fabric */
	short 	dfId;	        /* remote fabric */
	short 	dstPort;	/* Destination port in remote fabric */
	short 	domain;		/* Destination domain */
	u_int	ftr_sid;
	u_int	ftr_token;
	u_int	ftr_error;

#elif __BYTE_ORDER == __LITTLE_ENDIAN
	u_int	flags:24;	/* Flags */
	u_int	version:8;	/* pathinfo protocol version */
	char	error;		/* Error code */
	u_char	pType;		/* Port type */
	short	length;		/* Sequence total length */
	u_int	hdr_len:8;	/* Header length */
	u_int	d_id:24;	/* ID of the path destination's port */
	int	hops:8;		/* Residual hops */
	u_int	s_id:24;	/* ID of the path source's port */
	u_short	rsvd1;		/* Reserved */
	u_short msgId;		/* pathinfo query ID */
	short	rsvd2;		/* Reserved */
	short   srcRt;		/* Source Route pointer (offset) */
	short 	dfId;	        /* remote fabric */
	short 	sfId;	        /* source fabric */
	short 	domain;		/* Destination domain */
	short 	dstPort;	/* Destination port in remote fabric */
	u_int	ftr_sid;
	u_int	ftr_token;
	u_int	ftr_error;
#else
#error "Endianness undefined!"
#endif
} lsPATHI_hdr_t;

typedef struct {
	int destDomainId;
	int fabricID;
	int sourcePort;
	int destPort;
	int errMsgSize;
	char *errMsg;
	char basicStats[2];
	char extendedStats[2];
	char reversePath[2];
} dcm_in_parms_t;

enum {
	PATHINFO_DCM_IN_TX = 0,
	PATHINFO_DCM_OUT_TX,
	PATHINFO_DCM_IN_RX,
	PATHINFO_DCM_OUT_RX,
	PATHINFO_DCM_MAX
};
typedef struct {
	uint hop_count;
	uint in_port;
	uint out_port;
	uint domain;
	uint bandwidth;
	uint cost;
	uint short_interval;
	uint long_interval;
	uint is_reverse_path;
	uint bps_s[PATHINFO_DCM_MAX];
	uint bps_l[PATHINFO_DCM_MAX];
	uint tx_crdz_s[PATHINFO_DCM_MAX];
	uint tx_crdz_l[PATHINFO_DCM_MAX];
	uint fps_s[PATHINFO_DCM_MAX];
	uint fps_l[PATHINFO_DCM_MAX];
	char words[PATHINFO_DCM_MAX][STRING_64_LEN];
	char frames[PATHINFO_DCM_MAX][STRING_64_LEN];
	uint errors[PATHINFO_DCM_MAX];
	char input_tuples[STRING_64_LEN];
	char output_tuples[STRING_64_LEN];
} dcm_pathinfo_out;

typedef struct {
	uint num_hops;
	char status_msg[100];
	dcm_pathinfo_out *hop_pathinfo;
} dcm_out_parms_t;
/*
 * Function prototypes.
 */

extern inline int get_pathinfo_dcm(dcm_in_parms_t *dcm_params, char **buffer,
		int *size) {
	*buffer = (char *)calloc(1, 1);
	*size = 1;
	return (0);
}

extern inline int pack_out_pathinfo_dcm(lsPATHI_hdr_t *pathiP, int data_size,
dcm_in_parms_t *dcm_in_params, dcm_out_parms_t *dcm_params) {
	dcm_params->hop_pathinfo = (dcm_pathinfo_out *)calloc(
	                           sizeof (dcm_pathinfo_out), 2);
	memset(dcm_params->hop_pathinfo, 1, sizeof (dcm_pathinfo_out));
	memset(dcm_params->hop_pathinfo, 2, sizeof (dcm_pathinfo_out));
	return (2);
}

#endif // PATHINFO_H
