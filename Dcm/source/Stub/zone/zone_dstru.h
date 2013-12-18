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

#ifndef __ZONEDESTRU_H__
#define __ZONEDESTRU_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define eswap(_input_a, _input_b)

#define NZ_ACTIVE_CFG_SIGN      (unsigned int)0x900dface

typedef enum {
        NOERROR = 0,            /* generic no error */
        NOMEMORY,               /* generic malloc failure */
                                /* ZONE RULE CHECK ERROR CODE */
        EZACCEPT,               /* no zoning rule violation */
        EZBADPORT,              /* non-existent port number */
        FCTYPEMIX,              /* specific FC type and wildcard mix */
        ERSINGLEDEV,            /* more than one dev when LUN presents */
        EZLUNMIX,               /* mixture of devices w/ and w/o LUN */
                                /* at the same port */
        EZMENMIX,               /* mix of port and WWN zone members */
        EZHARDSOFTMIX,          /* mix of hard and soft zones */
        EZFAQLMIX,              /* mixing hard zoning with FA or QL zone */
        EZLUNMENMIX,            /* mix of QQQ */
        EZNEMENMIX,             /* mix of QQQ */
        EZLUNOFFSETFULL,        /* lun offset entries takes more than 4 */
                                /* ZONE TYPE MANAGEMENT ERROR CODE */
        ZT_SOFTZONE,            /* soft zoning - no need for ZT */
        ZT_FABASSIST,           /* FA zone - no need for ZT */
        ZT_QLOOPZONE,           /* quick loop zoning - no need for ZT */

        ZT_DRIVERERR,           /* driver returns error */
        ZG_NO_MORE_CAM,         /* no more CAM entry in port driver */
        ZCHECKBADWWN,           /* zone check bad WWN authentication */
        WWN_IN_PORTZONE,        /* wwn device in hard PORT zone */
        OFFSET_MASK_FULL,       /* no offset register available */
        PORT_IS_OFFLINE,        /* port is offline */
        ALL_PORTS_ARE_OFFLINE, /* all ports are offline */
        QLOOP_NOT_SUPPORTED,    /* QuickLoop not supported */
        PORT_EPORT,             /* port is an E-port */
        NO_CHANGE_NO_SAVE,      /* nothing changed, cfgSave is no-op */
        NO_CHANGE_NO_ENABLE,    /* nothing changed, cfgEnable is no-op */
        EMPTY_CFG_W_ENABLE,     /* no new cfg/zone/alias groups defined, */
                                /* but has an effective cfg. Happens when */
                                /* local cfg and neighbor's cfg differs */
                                /* by the effective cfg. */
        INDEX_TOO_BIG          /* a switch in the fabric cannot handle */
                                /* the d,i member */
} ezError;


#ifdef __cplusplus
}
#endif

using namespace WaveNs;

namespace DcmNs
{

}

#endif // __ZONEDESTRU_H__
