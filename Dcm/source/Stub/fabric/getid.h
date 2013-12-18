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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef __GETID_H__
#define __GETID_H__

using namespace WaveNs;

namespace DcmNs
{
UI32 getMyDomainIdFromVcsFabObjectManager();

static inline int getMyDomain (void)
{
    return getMyDomainIdFromVcsFabObjectManager ();
}

}

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MYSWITCH
#define MYSWITCH 1
#endif

#define MAX_MSG_SIZE 3724

#define ResourceInt 2

static inline int fabosInit(int inst)
{
    return (0);
}

static inline int ipcInit(const char *name, ushort inst, size_t maxMsgSize, ushort numberOfTypes)
{
    return (0);
}

static inline void configInit ()
{
}

static inline int configGet(const char *key, int type, void *value)
{
    return (1);
}

#ifdef __cplusplus
}
#endif

#endif // __GETID_H__


