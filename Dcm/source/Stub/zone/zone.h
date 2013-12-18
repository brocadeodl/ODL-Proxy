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

#ifndef __ZONEZONE_H__
#define __ZONEZONE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define TRANS_ABORT_MERGE_RECEIVED              -10
#define TRANS_ABORT_CFG_UPD_RECEIVED    -11
#define TRANS_ABORT_BAD_CFG                             -12
#define TRANS_ABORT_OPERATION_FAILED    -13
#define TRANS_ABORT_SHELL_EXITED                -14
#define TRANS_ABORT_UNKNOWN                             -15
#define TRANS_ABORT_USER_COMMAND                -16
#define TRANS_ABORT_DANGLING_XACT               -17
#define TRANS_ABORT_DNLD_DISRUPTIVE             -18
#define TRANS_ABORT_AD_ZNDB_FULL                -19
#define TRANS_ABORT_SYNCDUMP_OCCURRED   -20

#define ZN_TIMESTAMPSTR_SZ 30

#ifdef __cplusplus
}
#endif

using namespace WaveNs;

namespace DcmNs
{

}

#endif // __ZONEZONE_H__
