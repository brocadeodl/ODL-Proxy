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

#ifndef __NSCOMMON_H__
#define __NSCOMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* port type */
#define NSPT_UNKNOWN    0
#define NSPT_N_PORT 1
#define NSPT_NL_PORT    2
#define NSPT_NFL_PORT   3
/* 0x04-0x80 are reserved */
#define NSPT_Nx_PORT    0x7F    /* special value for all of the above */

#define NSPT_F_PORT 0x81
#define NSPT_FL_PORT    0x82
#define NSPT_LT_PORT    0x83
#define NSPT_E_PORT 0x84

#define NS_XBUF_LEN 524 /* lenth of scratch pad */

#define NSCOS_F     1   /* (1<<IU_CLASSF) */
#define NSCOS_1     2   /* (1<<IU_CLASS1) */
#define NSCOS_2     4   /* (1<<IU_CLASS2) */
#define NSCOS_3     8   /* (1<<IU_CLASS3) */

#define isNull(p) ((p) == 0)
#define nsfc4IsNull(p)  (isNull(p[0]) && isNull(p[1]) && isNull(p[2]) && \
             isNull(p[3]))

#ifndef NBBY
#define NBBY        8
#endif
#define NSFC4_NBPF  (sizeof (u_long) * NBBY)    /* # bits per field */

#define NSFC4_isSET(n, p)   ((p)[(n)/NSFC4_NBPF] & \
    ((unsigned)(1 << ((n) % NSFC4_NBPF))))

#define NS_DEV_HOST_TARGET_MASK     0x07
#define NS_DEV_PHY_VIR_MASK     0x38
#define NS_DEV_PHY_VIR_SHIFT        3

enum {
    NS_DEV_HOST_TARGET_UNK = 0,
    NS_DEV_HOST,
    NS_DEV_TARGET,
    NS_DEV_HOST_TARGET,
    NS_DEV_HT_MAX_TYPE
};

/* physical/virtual device type values, bit[5:3] of ns_devtype */
enum {
    NS_DEV_PHYSICAL = 0,
    NS_DEV_VIRTUAL,
    NS_DEV_NPV,
    NS_DEV_ISCSI,
    NS_DEV_PV_MAX_TYPE
};


#ifdef __cplusplus
}
#endif

#endif // __NSCOMMON_H__
