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


#ifndef __FOS_SWBD_H__
#define __FOS_SWBD_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Blade ID for fab control and fru header.
 * Terminator blade ID is applied to Cygnus as well.
 */
#define BLADE_ID_INITIALIZER            \
    ELARA2E_BLADE_ID=           0x52,   \
    ELARA2F_BLADE_ID=           0x53,   \
    CALLISTOE_BLADE_ID=         0x54,   \
    CALLISTOF_BLADE_ID=         0x5E,   \
    MERCURY_MM_BLADE_ID=        0x70,   \
	MERCURY_LC48X10G_BLADE_ID=  0x72,   \
    CARME_E_BLADE_ID=           0x76,   \
	MERCURY_LC12X40G_BLADE_ID=  0x7F,   \
	MERCURY_LC72X1G_BLADE_ID=   0x80,   \
	MERCURY_LC48X1G_BLADE_ID=   0x83,   \
    CASTOR_BLADE_ID=             0x87,   \
    POLLUX_BLADE_ID=            0x88,   \
    CASTORT_BLADE_ID=            0x91,   \
	LAST_BLADE_ID

/* Mercury and its descendents start with SWBD1000 */
#define SWBD1000    1000
#define SWBD1001    1001
#define SWBD1002    1002
#define SWBD1003    1003
#define SWBD1004    1004
#define SWBD1005    1005
#define SWBD1006    1006
#define SWBD1007    1007
#define SWBD1008    1008
#define SWBD1009    1009
#define SWBD1010    1010

/* Mercury and its descendents start with SWBD1000 */
#define SWBD_M4         SWBD1000    /* Mercury 4 line card slot system */
#define SWBD_M8         SWBD1001    /* Mercury 8 line card slot system */
#define SWBD_M16        SWBD1002    /* Mercury 16 line card slot system */
#define SWBD_RESERVED   SWBD1003    /* Mercury XX line card slot system */
#define SWBD_LC48X10G   SWBD1004    /* 48 port 10G Line card */
#define SWBD_LC12X40G   SWBD1005    /* 12 port 40G Line card */
#define SWBD_LC72X1G    SWBD1006    /* 72 port 1G Line card */
#define SWBD_LC48X1G    SWBD1007    /* 48 port 1G Line card */

/* Callisto and Carme */
#define SWBD95 95 /* Elara2E */
#define SWBD96 96 /* Elara2F */
#define SWBD97 97 /* CallistoE */
#define SWBD107 107 /* CallistoF */
#define SWBD116 116 /* CarmeE */
#define SWBD131 131 /* CarmeE */
#define SWBD132 132 /* CarmeE */
#define SWBD137 137 /* CarmeE */

#define SWBD_ELARA2E SWBD95
#define SWBD_ELARA2F SWBD96
#define SWBD_CALLISTOE SWBD97
#define SWBD_CALLISTOF SWBD107
#define SWBD_CARME_E SWBD116
#define SWBD_CASTOR SWBD131
#define SWBD_POLLUX SWBD132
#define SWBD_CASTORT SWBD137

#ifdef __cplusplus
}
#endif

#endif /* __FOS_SWBD_H__ */
