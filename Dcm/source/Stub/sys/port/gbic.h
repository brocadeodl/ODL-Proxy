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
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 * GBIC header
 */

#ifndef __SYS_GBIC_H__
#define	__SYS_GBIC_H__

#define I2C_START	0x0001 /* issue a start condition */
#define I2C_STOP	0x0002 /* issue a stop condition */
#define I2C_READ	0x0003 /* issue a start condition */
#define I2C_WRITE	0x0004 /* issue a stop condition */

#define GBIC_TTL        300     /* time-to-live for cached data in secs */

/*
 * Valid definitions for gbic valid field. No need to change the FSS version
 * to be compatible with the previous versions since the check on this field
 * has been done on either 0 or not and should work with previous versions.
 */
#define GBIC_NEW        0
#define GBIC_GOOD       1
#define GBIC_BAD        2

/*
 * GBIC type definitions - MOD_DEF bits from GBIC, flipped
 */
typedef enum {
	GBIC_NONE,		/* GBIC not present */
	GBIC_SERIAL,		/* Serial Identification */
	GBIC_CU_INTRA,		/* Copper Intra-enclosure */
	GBIC_LW_GE,		/* Optical Longwave 1.25 Gbd */
	GBIC_CU_INTER,		/* Copper Inter-enclosure */
	GBIC_SW_FC,		/* Optical Shortwave */
	GBIC_LW_FC,		/* Optical Longwave */
	GBIC_SW_GE,		/* Optical Shortwave 1.25 Gbd */
	GBIC_IP			/* Virtual E_Port */
} gbic_def;

/*
 * GBIC type definitions - SNMP
 */
typedef enum {
	GBIC_UNKNOWN = 1,	/* Type Unknown */
	GBIC_LONGWAVE,		/* Longwave Laser */
	GBIC_SHORTWAVE,		/* Shortwave Laser */
	GBIC_LED,		/* Light Emitting Diode */
	GBIC_ELECTRICAL,	/* Copper */
	GBIC_SHORTWAVE_OFC	/* Shortwave Laser with OFC */
} gbic_type;


/*
 * Port Media Types (as defined in table 32 of XFP spec rev 3.1)
 */
typedef enum media_type {
	MEDIA_UNKNOWN	=	0x0,
	MEDIA_GBIC		=	0x1,
	MEDIA_FIXED		=	0x2,
	MEDIA_SFP		=	0x3,
	MEDIA_XFP		=	0x6,
	MEDIA_XFF		=	0x7,
	MEDIA_XFP_E		=	0x8,
	MEDIA_DWDM_SFP	=	0xB,
	MEDIA_TYPE_LAST	=	0xC
} media_type_t;

/*
 * PTIO_MEDIA_STATE arg2 definitions (see proto_physm.h/c)
 */
typedef enum ptio_media_state_arg_defs {
	MEDIA_IS_UNKNOWN	=	0x0,
	MEDIA_IS_COPPER		=	0x1,
	MEDIA_IS_DWDM		=	0x2 
} ptio_media_state_arg_defs_t;


/*
 * Definition of the bits in the 4th byte of the "transceiver" field
 * if SFP
 */
#define SFP_VLONG_DIST   0x80
#define SFP_SHORT_DIST   0x40
#define SFP_INTERM_DIST  0x20
#define SFP_LONG_DIST    0x10
#define SFP_LONGWAVE_LC  0x02
#define SFP_INTER_EL     0x01  /* electrical inter-enclosure */


/*
 * Definition of the bits in the 5th byte of the "transceiver" field
 * if SFP
 */
#define SFP_INTRA_EL     0x80  /* electrical intra-enclosure */
#define SFP_SHORTWAVE_N  0x40  /* shortwave laser w/o OFC */
#define SFP_SHORTWAVE_L  0x20  /* shortwave laser w/ OFC */
#define SFP_LONGWAVE_LL  0x10

/*
 * Definition of the bits in the 6th byte of the "transceiver" field
 * if SFP
 */
#define SFP_MEDIA_TW     0x80
#define SFP_MEDIA_TP     0x40
#define SFP_MEDIA_MI     0x20
#define SFP_MEDIA_TV     0x10
#define SFP_MEDIA_M6     0x08
#define SFP_MEDIA_M5     0x04
#define SFP_MEDIA_SM     0x01


/*
 * Definition of the bits in the first byte of the "transceiver" field
 * if XFP
 */
#define XFP_MX_SN     0x80 /* multimode short distance */
#define XFP_SM_LL     0x40 /* single mode longwave */
#define XFP_EXTEND_LL 0x20 /* extended reach 1550nm */
#define XFP_INTERM_LL 0x10 /* intermediate reach 1300nm */

/*
 * Definition of the bits in the 8th byte of the "transceiver" field
 * of SFP
 */
#define TRANSCEIVER_FC_SPEED_BYTE	7 /* 8th byte "transceiver" field */

#define TRANSCEIVER_SPEED_1G	0x01
#define TRANSCEIVER_SPEED_2G	0x04
#define TRANSCEIVER_SPEED_4G	0x10
#define TRANSCEIVER_SPEED_8G	0x40

/*
 * Definition of the bits in the 4th byte of the "transceiver" field
 * of SFP for GE Port Compliance
 */
#define TRANSCEIVER_GE_SPEED_BYTE   3 /* 4th byte "transceiver" field */

/*
 *   In the tranceiver byte described above based on the 8472 spec,
 *	 Bit 0 set indicates SFP is 1000Base-SX, 
 *	 Bit 1 - SFP is 1000BaseLX,
 *	 Bit 2 - SFP is 1000BaseCX,
 *	 Bit 3 - SFP is 1000BaseT.
 *	 So, for our media validation, if any of these bits is set, 
 *   the SFP supports 1G speed. 
 */
#define TRANSCEIVER_GE_SPEED_1G    0x0F

#define TRANSCEIVER_GE_10G_SPEED   0x04

/*
 * 4th byte of serial data or first byte of transceiver field.
 */
#define TRANSCEIVER_10G_SPEED_BYTE	0
#define TRANSCEIVER_GE_SPEED_10G_SR 0x10
#define TRANSCEIVER_GE_SPEED_10G_LR 0x20
/* Refer SFP-8472 Draft Spec */
#define TRANSCEIVER_GE_COPPER_PASSIVE_SPEED_10G  	0x1


/*
 * Encoder 10G bit definition for the XFP
 */
#define ENCODER_SPEED_10G			0x80

/*
 * Gbic data structure
 * IMPORTANT:
 * make sure this structure matches media.h as well
 * structure is synchronized across FSS, make sure that is
 * taken into account if you change the size of the structure
 */
typedef struct {
	int	state;		/* last state change */
	int	state_trans_num;		/* state change #'s */


	u_char	warn_flags[2];	/* High and Low Warn flags only set for smart gbics */
	u_char	available;	/* free for use */
	u_char	valid;		/* cached data valid */
	time_t	secs;		/* cache time stamp */

	/*
	 * serial data
	 */
	char	identifier;	/* Type of serial transceiver */
	char	res1;		/* Reserved */
	char	connector;	/* Connector type */
	char	transceiver[8];	/* Transceiver compatibility */
	char	encoding;	/* Serial encoding algorithm */
	char	baud_rate;	/* Nominal baud rate */
	char	res2;		/* Reserved */
        char    len9m_km;       /* Link length supported for 9m fibre (km) */
	char	len9um;		/* Link length supported for 9um fibre */
	char	len50um;	/* Link length supported for 50um fibre */
	char	len625um;	/* Link length supported for 62.5um fibre */
	char	lencu;		/* Link length supported for copper */
	char	res3;		/* Reserved */
	char	vendor[16];	/* Vendor name (ASCII) */
	char	res4;		/* Reserved */
	char	oui[3];		/* Vendor IEEE company ID */
	char	partno[16];	/* Part number (ASCII) */
	char	rev[4];		/* Revision level (ASCII) */
        char    wavelength[2];  /* Laser wavelength */
	char	res5;		/* Reserved */
	char	cc_base;	/* Check code for base ID fields */

	char	options[2];	/* Optional signal implementation */
	char	br_max;		/* Upper baud rate margin */
	char	br_min;		/* Lower baud rate margin */
	char	serialno[16];	/* Serial number (ASCII) */
	char	date_code[8];	/* Vendor's manufactoring date code */
	char	res6[3];	/* Reserved */
	char	cc_ext;		/* Check code for extended ID fields */

	u_char	alarm_flags[2]; /* High and Low Alarm flags  only set if smart is set*/
	u_char	status_ctrl;	/* Optional Status/Control Bits only available for smart gbics*/
	u_char	smart;			/* Smart GBIC */

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

} gbic_t;

/*
 * structure to passback gbic information for famib
 */
typedef struct {
	int	*type;
	char	*sn;
	char	*rev;
	char	*vendor;
} gbic_fa_t;

#endif /* __SYS_GBIC_H__ */
