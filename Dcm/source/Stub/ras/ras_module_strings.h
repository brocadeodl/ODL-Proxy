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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Description:                                                          *    
 *       This header file contains RAS-LOG related stub routines           *
 *       and structures                                                    *    
 ***************************************************************************/

#ifndef __RASLOGMODULES_H__
#define __RASLOGMODULES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RAS_UNKN_ID     (0)         /*  UNKNOWN MODULE */
#define RAS_KT_ID       (1)         /*  Kernel Test ID */
#define RAS_MODULE_ID_MIN   (RAS_KT_ID)
#define RAS_EM_FSS_ID   (293)   /* emd FSS Client trace */
#define RAS_MODULE_ID_MAX (RAS_EM_FSS_ID)

unsigned char trace_id_string_array[RAS_MODULE_ID_MAX + 1][8];

        /*  Macro to convert from Module ID to string.
         *
         *  This macro returns a pointer to a string containing the module
         *  ID string.  If the module is invalid, it returns unknown module.
         */
#define RAS_MODULE_STRING(x)  ( ( (x) <= 0) || ( (x) > RAS_MODULE_ID_MAX) ) ? \
                              trace_id_string_array[0] : \
                              trace_id_string_array[x]

#ifdef __cplusplus
}
#endif

#endif
