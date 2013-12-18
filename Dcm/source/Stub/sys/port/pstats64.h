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
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Description:
 *      64-bit counter and port interface.
 *
 */

#ifndef __SYS_STAT64_H__
#define	__SYS_STAT64_H__

#if 0
#ifndef __KERNEL__
#include <sys/types.h>
#else
#include <linux/types.h>
#endif /* __KERNEL__ */

#include <sys/fabos/cdefs.h>
#include <sys/port/admin.h>

OS_CDECLS_BEGIN
#endif
/*
 * This controls the rate of statistics sampling in switch_poll_ports.
 * Polling must be frequent enough to avoid 32-bit counter overflow
 * for errors and tx/rx bytes, etc. */
#define RATE_SAMPLE_PERIOD	5	/* seconds */

/* 64-bit counter structures */

typedef struct {
	u_int top_int;
	u_int bottom_int;
} long64_t;

typedef struct {

	/* ****these rates are calulated values******* */
	u_int rateTxFrame;
			/* Tx instantaneous frame rate (fr/sec) */
	u_int rateRxFrame;
			/* Rx instantaneous frame rate (fr/sec) */
	u_int rateTxPeakFrame;
			/* Tx max frame rate since last reset -  fr/sec */
	u_int rateRxPeakFrame;
			/* Rx max frame rate since last reset -  fr/sec */
	u_int rateRxByte;
			/* Rx instantaneous byte rate (bytes/sec) */
	u_int rateTxByte;
			/* Tx instantaneous byte rate (bytes/sec) */
	u_int rateTxPeakByte;
			/* Tx max byte rate since last reset -  bytes/sec */
	u_int rateRxPeakByte;
			/* Rx max byte rate since last reset -  bytes/sec */

	/* 64-bit port object and error object counters */
	long64_t error;
			/* # of errors on this port */
	long64_t txFrame;
			/*
			 * No. of frames/packets/IOs that have been
			 * transmitted by this port
			 */
	long64_t rxFrame;
			/*
			 * no. of frames/packets/IOs that have been
			 * received by this port
			 */
	long64_t txOctets;
			/*
			 * No. of octets or bytes that have been
			 * transmitted by this port
			 */
	long64_t rxOctets;
			/*
			 * No. of octets or bytes that have been
			 * received by this port
			 */
	long64_t BBCreditZero;
			/* No. of transitions in/out of BBcredit zero state */
	long64_t inputBuffersFull;
			/*
			 * No. of occurances when all input buffers full and
			 * outbound b-tob credit transitioned to zero
			 */
	long64_t FBSYFrames;
			/*
			 * No. of times FBSY returned to this port due to
			 * dest. port busy
			 */
	long64_t PBSYFrames;
			/*
			 * No. of times PBSY returned to this port
			 * because dest port busy
			 */
	long64_t FRJTFrames;
			/*
			 * No. of times FRJT returned to port
			 * because frame rejected by fabric
			 */
	long64_t PRJTFrames;
			/*
			 * No. of times PRJT returned to port
			 * because frame rejected by dest. N_Port
			 */
	long64_t rxClass1Frames;
			/* No. of class 1 frames rec. at this port */
	long64_t rxClass2Frames;
			/* # of class 2 frames rec. at this port */
	long64_t rxClass3Frames;
			/* # of class 3 frames rec. at this port */
	long64_t class3Discards;
			/*
			 * No. of class 3 frames discarded upon
			 * reception at this port
			 */
	long64_t rxMcastFrames;
			/* # of multicast frames or packets rec. at this port */
	long64_t txMulticastFrames;
			/*
			 * No. of multicast frames or packets transmitted
			 * out this port
			 */

	/* port's error counters From FA MIB 3.0 */
	long64_t portErrorReset;
			/* setting this clears all error counters */
	long64_t rxLinkResets;
			/* # of link resets received */
	long64_t txLinkResets;
			/* # of link resets transmitted */
	long64_t rxOfflineSequences;
			/* # of primitive OLS rec. at this port */
	long64_t txOfflineSequences;
			/* # of primitive OLS transmitted by this port */
	long64_t linkFailures;
			/*
			 * # of link failures.
			 * Part of Link Error Status (LESB)
			 */
	long64_t invalidCRC;
			/*
			 * # of frames received with invalid CRC.
			 * Part of LESB
			 */
	long64_t invalidTxWords;
			/* # of invalid transmission words rec. at this port */
	long64_t primSeqProtoErrors;
			/*
			 * No. of primitive sequence protocol errors detected
			 * at this port
			 */
	long64_t signalLosses;
			/*
			 * # of instances of signal loss detected at this port
			 */
	long64_t syncLosses;
			/*
			 * No. of instances of synchronization loss detected
			 * at this port
			 */
	long64_t invalidOrderedSets;
			/* # of invalid ordered sets rec. at this port */
	long64_t rxFramesTooLong;
			/*
			 * No. of frames rec. at this port with frame length
			 * longer than expected
			 */
	long64_t rxTruncFrames;
			/*
			 * No. of frames rec. at this port whose frames
			 * length shorter than minimun specified in header
			 */
	long64_t addressIdErrors;
			/* # or frames rec. with unknown addressing */
	long64_t delimiterErrors;
			/* # of invalid frame delimiters rec. at this port */
	long64_t rxEncodingDisparityErr;
			/* # of disparity errors rec. at this port */

	/* from SW MIB */
	long64_t tooManyRdys;
			/* # of RDYs exceeding # of frames received */
	long64_t rxCRCs;
			/* # of CRC errors detected for frames received */
	long64_t rxBadEofs;
			/* # of received frames with bad EOFs */
	long64_t rxEncOutFrames;
			/*
			 * No.  of encoding error or disparity error
			 * outside frames received
			 */
	long64_t mcastTimedOuts;
			/* # of multicast frames that have been timed out */
	long64_t rxLCs;
			/* # of link control frames received at this port */
} port64_t;

typedef struct {
	u_int error;
	u_int txFrame;
	u_int rxFrame;
	u_int txOctets;
	u_int rxOctets;
	u_int BBCreditZero;
	u_int inputBuffersFull;
	u_int FBSYFrames;
	u_int PBSYFrames;
	u_int FRJTFrames;
	u_int PRJTFrames;
	u_int rxClass1Frames;
	u_int rxClass2Frames;
	u_int rxClass3Fames;
	u_int class3Discards;
	u_int rxMcastFrames;
	u_int txMulticastFrames;
	u_int rxLinkResets;
	u_int txLinkResets;
	u_int rxOfflineSequences;
	u_int txOfflineSequences;
	u_int linkFailures;
	u_int invalidCRC;
	u_int invalidTxWords;
	u_int primSeqProtoErrors;
	u_int signalLosses;
	u_int syncLosses;
	u_int invalidOrderedSets;
	u_int rxFramesTooLong;
	u_int rxTruncFrames;
	u_int addressIdErrors;
	u_int delimiterErrors;
	u_int rxEncodingDisparityErr;
	u_int tooManyRdys;
	u_int rxCRCs;
	u_int rxBadEofs;
	u_int rxEncOutFrames;
	u_int mcastTimedOuts;
	u_int rxLCs;
} port32_t; /* all the HW counter values */


#endif /* __SYS_STAT64_H__ */
