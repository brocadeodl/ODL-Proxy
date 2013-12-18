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
 *    Copyright (c) 1996-2004 Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: gbic.h
 *
 *    Description:
 *	FabOS user gbic library header file.
 */

#ifndef __GBIC_H__
#define	__GBIC_H__

#include <sys/port/gbic.h>
#include <sys/port/media.h>
#include <sys/fabos/mmap.h>
#if 0
#include <sys/fabos/cdefs.h>

#include <switch/ioctl.h>
#include <switch/scn.h>
#include <fkio/port.h>
#endif


/*
 * register to receive GBIC status changes
 */
#define gbicRegister(func, msg_q)	scnRegister(GBIC_SCN, func, msg_q)
#define gbicDeRegister(func, msg_q)	scnDeRegister(GBIC_SCN, func, msg_q)

#define gbicModuleType(port) fgbicModuleType(FabOS_SW_FD, port)
#define geGbicModuleType(port) fgeGbicModuleType(FabOS_SW_FD, port)

/*
 * gbic library function prototypes:
 */
static inline gbic_t *	gbicSerialId(int port , gbic_t * pgic){
	static gbic_t mgbic;
	return (&mgbic);
}

extern	char *		gbicName(int);
extern	int		gbicCopper(int);
extern	int		geGbicCopper(int);
extern	int		gbicTxType(int port);
static	const char *		gbicIdType(gbic_t *) {
	return ("");
}
static	const char *		gbicIdConnector(gbic_t *) {
	return ("");
}
extern	int		gbicFASID(int,  gbic_fa_t *);
static	const char *		gbicIdName(gbic_t *) {
	return ("");
}
static inline	const char *		gbicIdSpeed(gbic_t *pgbic){
	return ("UNKNOWN");
}
static const char *		gbicIdMedia(gbic_t *) {
	return ("");
}
static	const char *		gbicIdEncoding(gbic_t *) {
	return ("");
}
static	const char *		gbicIdDistance(gbic_t *) {
	return ("");
}
static	const char *		gbicIdOptions(gbic_t *) {
	return ("");
}
extern	void		gbicShow(int, int);
extern  int		gbicTransCount(int);

/*
 * gbic library for GiGE
 */
//extern	gbic_t *	gbicGeSerialId(int, gbic_t *);



extern int media_get_identifier(media_data_t *media, char *id);
extern int media_get_vendor(media_data_t *media, char **vendor);
extern int media_get_oui(media_data_t *media, char **oui);
extern int media_get_partno(media_data_t *media, char **partno);
extern int media_get_rev(media_data_t *media, char **rev);
extern int media_get_temperature(media_data_t *media, int *temp);
extern int media_get_tx_power(media_data_t *media, int *tx_power);
extern int media_get_rx_power(media_data_t *media, int *rx_power);
extern int media_is_ELWL(media_data_t *media);



#endif	/* __GBIC_H__ */
