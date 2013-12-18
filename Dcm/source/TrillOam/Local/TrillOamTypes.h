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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mmahajan                                                     *
 **************************************************************************/

#ifndef TRILLOAMTYPESH_H
#define TRILLOAMTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        TRILLOAMTRILLOAMPINGREQUEST = 0 ,
        TRILLOAMTRILLOAMTRACEROUTEREQUEST
    }TrillOam;

}

typedef struct trilloam_msg_traceroute_resp_row_ {
    unsigned short  egid;           /* Egress logical port */
    unsigned short  trill_da;       /* Trill nick name  */
    unsigned char   ingress[64];    /* ingress interface */
    unsigned char   egress[64];     /* egress interface */
    unsigned int    rttime;         /* round trip time */
} trilloam_msg_traceroute_resp_row;

typedef struct trilloam_msg_traceroute_resp_ {
    unsigned int   cmd;
    unsigned int   size;
    unsigned int   sessionid;
    unsigned int   begin;
    unsigned int   last;
    unsigned int   row_count;
    trilloam_msg_traceroute_resp_row rows[1];
}trilloam_msg_traceroute_resp;

#endif                                            //TRILLOAMTYPESH_H
