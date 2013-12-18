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
 *   Author : Meenakshi Garg                                               *
 *   Description:                                                          *    
 *       This header file contains fspf IPC related stub routines          *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef FSPF_IPC_H
#define FSPF_IPC_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
        int     t_domain;
        int     t_metric; /* Lowest of the metrics in pdbs */
        char    t_name[32]; /* length of the inquiry string */
        int     t_uHopsFromRoot; 
        int     t_pathCnt;
        int t_bps; /* bits per second */
} topo_ipc_t;

#ifdef __cplusplus
}               
#endif          
                
#endif // FSPF_IPC_H
