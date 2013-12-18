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
 *    Copyright (c) 1996-2009 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 * GBIC header
 */

#ifndef __SYS_MEDIA_H__
#define	__SYS_MEDIA_H__

#include "gbic.h"

#define MEDIA_MMAP_ADDR    0x50
#define MEDIA_MMAP_SCP     0x51

#define MEDIA_DATA_SIZE_MAX  256
#define MEDIA_SMART_DATA_SIZE     128
#define MEDIA_SERIAL_DATA_SIZE     128

#define MEDIA_IS_SMART         0x40
#define SFP_INTERN_CAL         0x20
#define SFP_EXTERN_CAL         0x10

#define SFP_SMART_DATA_ADDR    0
#define SFP_DIGIT_DIAG_ADDR     96
#define SFP_DIAG_TYPE_ADDR      92
#define SFP_CC_BASE_ADDR        63
#define SFP_CC_EXT_ADDR         95
#define SFP_OPT_STAT_CTRL_ADDR	110
#define SFP_ALARM_FLAG_ADDR		112
#define SFP_WARN_FLAG_ADDR		116


#define SFP_DIGIT_DIAG_SIZE		10	

#define SFP_TEMP_DATA_SIZE     8
#define SFP_THRESH_DATA_SIZE     32

/* address defines for XFP */
#define XFP_PAGE_SEL_ADDR      127
#define XFP_PAGE_SEL_SERIAL_ID   0x1
#define XFP_SERIAL_ID_ADDR     128
#define XFP_SMART_DATA_ADDR    0

#define MEDIA_GET_ALL            0x01
#define MEDIA_GET_SMART_DATA     0x02

/* command set for XFP register control */
#define XFP_CLR_INTR_ALL         0x03
#define XFP_RESET_COMPLETE_ONLY  0x04
#define XFP_CLR_RESET_COMPLETE   0x05
#define XFP_SET_DATA_RATE        0x06

#define XFP_INT_FLAG_RESET_COMP 84
#define XFP_INT_FLAG_BYTES    8
#define XFP_INT_MASK_ADDR    88
#define XFP_INT_MASK_BYTES    8
#define XFP_SIGNL_CTRL_ADDR     1

#define L1310NM       1310

typedef enum {
	MEDIA_BAD_BUFS =		-1001,
	MEDIA_SMART_BAD_BUFS =	-1002,
	MEDIA_BASE_CKSUM_ERR =	-1003,
	MEDIA_EXT_CKSUM_ERR =	-1004,
	MEDIA_SMART_CKSUM_ERR =	-1005,	// Not currently used
	MEDIA_SMART_TYPE_ERR =	-1006,
	MEDIA_SMART_INV_DATA = 	-1007

} media_data_errors;

/*
 * Serial ID data fields for SFP
 * total 96 bytes
 */
typedef struct media_sfp_serialID {
	/* byte 0 - 63 */
	u_char	identifier;	/* Type of serial transceiver */
	u_char	ext_id;		/* Extended id */
	u_char	connector;	/* Connector type */
	u_char	transceiver[8];	/* Transceiver compatibility */
	u_char	encoding;	/* Serial encoding algorithm */
	u_char	baud_rate;	/* Nominal baud rate */
	u_char    res1;           /* Reserved */
	u_char    len9umk;        /* Link length supported for 9um fibre, in km */
	u_char	len9um;	/* Link length supported for 9um fibre, in 100m */
	u_char	len50um;	/* Link length supported for 50um fibre */
	u_char	len625um;	/* Link length supported for 62.5um fibre */
	u_char	lencu;		/* Link length supported for copper */
	u_char	res2;		/* Reserved */
	u_char	vendor[16];	/* Vendor name (ASCII) */
	u_char	res3;		/* Reserved */
	u_char	oui[3];		/* Vendor IEEE company ID */
	u_char	partno[16];	/* Part number (ASCII) */
	u_char	rev[4];		/* Revision level (ASCII) */
	u_char  wavelen[2];  /* Laser wavelength */
	u_char	res4;	        /* Reserved */
	u_char	cc_base;	/* Check code for base ID fields */
	/* byte 64 - 95 */
	u_char	options[2];	/* Optional signal implementation */
	u_char	br_max;		/* Upper baud rate margin */
	u_char	br_min;		/* Lower baud rate margin */
	u_char	serialno[16];	/* Serial number (ASCII) */
	u_char	date_code[8];	/* Vendor's manufactoring date code */
	u_char  diag_type;      /* diagnostic monitoring type */
	u_char  enh_options;    /* enhanced options implemented */
	u_char	res5;	        /* Reserved */
	u_char	cc_ext;		/* Check code for extended ID fields */
} media_sfp_serialID_t;

/*
 * Serial ID data fields for XFP
 * Page select 0x1, address starts from 128
 * total 96 bytes
 */
typedef struct media_xfp_serialID {
	/* offset from 128, byte 0 - 63 */
	u_char	identifier;	/* Type of serial transceiver */
	u_char	ext_id;		/* Extended id */
	u_char	connector;	/* Connector type */
	u_char	transceiver[8];	/* Transceiver compatibility */
	u_char	encoding;	/* Serial encoding algorithm */
	u_char	baud_min;	/* Minimal bit rate, in 100 MBits/s */
	u_char	baud_max;	/* Maximum bit rate, in 100 MBits/s */
	u_char    len_smf;         /* Link length supported for SMF, in km */
	u_char	len_ebw;	/* Link length supported for EBW 50 um */
	u_char	len50um;	/* Link length supported for 50um fibre */
	u_char	len625um;	/* Link length supported for 62.5um fibre */
	u_char	lencu;		/* Link length supported for copper */
	u_char	dev_tech;	/* Device technology */
	u_char	vendor[16];	/* Vendor name (ASCII) */
	u_char	cdr_rate;	/* CDR rate support */
	u_char	oui[3];		/* Vendor IEEE company ID */
	u_char	partno[16];	/* Part number (ASCII) */
	u_char	rev[2];		/* Revision level (ASCII) */
	u_char    wlengh[2];      /* Nominal laser wavelengh, value/20 in nm */
	u_char    wrange[2];      /* Range of laser wavelength from norm */
	u_char    max_ctemp;      /* Maximum case temperature in Celsius */
	u_char	cc_base;	/* Check code for base ID fields */
	/* offset from 128, byte 64 - 95 */
	u_char    psu[4];            /* Power supply current & power requirements */
	u_char	serialno[16];	/* Serial number (ASCII) */
	u_char	date_code[8];	/* Vendor's manufactoring date code */
	u_char    diag_mtype;     /* Diag monitor type */
	u_char    opt_feature;    /* Optional enhanced feature in transceiver */
	u_char    aux_mon;        /* Define quantities for Aux. A/D channels */
	u_char	cc_ext;		/* Check code for extended ID fields */
} media_xfp_serialID_t;


/*
 * xfp smart data, stored in lower memory map addr 0-127
 */
typedef struct media_xfp_smart_data {
	u_char   identifier;    /* Module identifier of the physical device */
	u_char   sig_ctrl;      /* Signal conditioner control */
	/* 2-57 (56 bytes), threshold used for alarm and warning flags */
	short	temp_high_alrm; /* Temp High Alarm */
	short	temp_low_alrm;	/* Temp Low Alarm */
	short	temp_high_warn;	/* Temp High Warning */
	short	temp_low_warn;	/* Temp Low Warning */

	u_char    res_ad_thresh[8]; /* Reserved A/D flag threshold */

	unsigned short	bias_high_alrm; /* Bias High Alarm */
	unsigned short	bias_low_alrm;	/* Bias Low Alarm */
	unsigned short	bias_high_warn;	/* Bias High Warning */
	unsigned short	bias_low_warn;	/* Bias Low Warning */

	unsigned short	txpwr_high_alrm;/* Tx power High Alarm */
	unsigned short	txpwr_low_alrm;	/* Tx power Low Alarm */
	unsigned short	txpwr_high_warn;/* Tx power High Warning */
	unsigned short	txpwr_low_warn;	/* Tx power Low Warning */

	unsigned short	rxpwr_high_alrm;/* Rx power High Alarm */
	unsigned short	rxpwr_low_alrm;	/* Rx power Low Alarm */
	unsigned short	rxpwr_high_warn;/* Rx power High Warning */
	unsigned short	rxpwr_low_warn;	/* Rx power Low Warning */

	unsigned short   aux1_high_alrm; /* Aux 1 high alarm */
	unsigned short   aux1_low_alrm;  /* Aux 1 low alarm */
	unsigned short   aux1_high_warn; /* Aux 1 high warning */
	unsigned short   aux1_low_warn;  /* Aux 1 low warning */

	unsigned short   aux2_high_alrm; /* Aux 2 high alarm */
	unsigned short   aux2_low_alrm;  /* Aux 2 low alarm */
	unsigned short   aux2_high_warn; /* Aux 2 high warning */
	unsigned short   aux2_low_warn;  /* Aux 2 low warning */

	u_char    vps_reg[2];     /* Optional VPS control registers */
	u_char    res1[10];       /* Reserved */
	u_char    ber_report[2];  /* BER reporting */
	u_char    wctrl_reg[4];   /* Wavelength control registers */
	u_char    fctrl_reg[4];   /* FEC control registers */
	u_char    intr_ctrl[16];  /* Flags and interrupt control */
	/*  96 - 109, A/D readout (14 bytes) */
	short	temperature;	/* Module temperature (degrees C) */
	unsigned short   res2;           /* Reserved */
	unsigned short   tx_bias;        /* Tx bias current */
	unsigned short	tx_power;	/* Tx output power */
	unsigned short   rx_power;       /* Rx input power */
	unsigned short   aux1;           /* Auxiliary measurement 1 */
	unsigned short   aux2;           /* Auxiliary measurement 2 */
	u_char    ctrl_bit[2];    /* Control and status bits */
	u_char    res3[6];        /* Reserved */
	u_char    rw_err;         /* Serial interface read/write error checking */
	u_char    passwd_change[4]; /* Passwd change entry area */
	u_char    passwd[4];      /* passwd entry area */
	u_char    page_select;    /* Page select byte */
} media_xfp_smart_data_t;


typedef struct media_sfp_smart_data {
	/*
	 * Alarm and warning thresholds
	 */
	short	temp_high_alrm; /* Temp High Alarm */
	short	temp_low_alrm;	/* Temp Low Alarm */
	short	temp_high_warn;	/* Temp High Warning */
	short	temp_low_warn;	/* Temp Low Warning */

	unsigned short	volt_high_alrm; /* Voltage High Alarm */
	unsigned short	volt_low_alrm;	/* Voltage Low Alarm */
	unsigned short	volt_high_warn;	/* Voltage High Warning */
	unsigned short	volt_low_warn;	/* Voltage Low Warning */

	unsigned short	bias_high_alrm; /* Bias High Alarm */
	unsigned short	bias_low_alrm;	/* Bias Low Alarm */
	unsigned short	bias_high_warn;	/* Bias High Warning */
	unsigned short	bias_low_warn;	/* Bias Low Warning */

	unsigned short	txpwr_high_alrm;/* Tx power High Alarm */
	unsigned short	txpwr_low_alrm;	/* Tx power Low Alarm */
	unsigned short	txpwr_high_warn;/* Tx power High Warning */
	unsigned short	txpwr_low_warn;	/* Tx power Low Warning */

	unsigned short	rxpwr_high_alrm;/* Rx power High Alarm */
	unsigned short	rxpwr_low_alrm;	/* Rx power Low Alarm */
	unsigned short	rxpwr_high_warn;/* Rx power High Warning */
	unsigned short	rxpwr_low_warn;	/* Rx power Low Warning */

	/*
	 * Internally calibrated diagnostic data
	 */
	int	temperature;	/* Module temperature (degrees C) */
	int	voltage;        /* Supply voltage (Volts) */
	int	opt_current;	/* Laser diode drive current (milliAmps) */
	int	tx_power;	/* Transmitted optical power (microWatts) */
	int	rx_power;	/* Received optical power (microWatts) */
} media_sfp_smart_data_t;


/*
 * media data structure, structure aligned with gbic_t for SFP
 */
typedef struct media_data {
	int	state;		/* last state change */
	int	state_trans_num;		/* state change #'s */
	u_char	warn_flags[2];			/* High and Low Warn flags */
	u_char available;	/* not used, buffer to match otehr structure */
	u_char	valid;		/* cached data valid */
	time_t	secs;		/* cache time stamp */
	int unit;
	int cmd;                /* read serial ID and smart data or
				   read smart data only */


	union { /* 96 bytes */
		media_sfp_serialID_t sfp_data;
		media_xfp_serialID_t xfp_data;
	} serial_data;

	u_char	alarm_flags[2];			/* High and Low Alarm flags */
	u_char	status_ctrl;			/* Optional Status/Control Bits */
	u_char	smart;		/* Smart GBIC */

	union {
		media_sfp_smart_data_t sfp_s_data;
		media_xfp_smart_data_t xfp_s_data;
	} smart_data;
} media_data_t;

extern int gbic_from_media(media_data_t *media, gbic_t *gbic);
extern int media_is_copper(media_data_t *media);

extern int media_smart_from_buf(u_char *buf, media_data_t *media);
extern int media_data_from_buf(u_char *buf, media_data_t *media);
#endif
