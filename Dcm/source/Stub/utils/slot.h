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

#ifndef __SLOT_H__
#define __SLOT_H__

#include <stdlib.h>
#include "Framework/Utils/FrameworkToolKit.h"
#include "Stub/sysmod/syscomm.h"

#ifdef __cplusplus
extern "C"
{
#endif

static inline int slot_id2no(int type, int id)
{
	printf("*****slot_id2no type %d id %d\n", type, id);
	return (4);
}
static inline int slot_no2id(int vno, int *type, int *pno)
{
	*pno = vno;
    *type = 0x10;
    return (0);
}

static inline int slot_s2no(char *s)
{
    return atoi(s);
}

static inline const char  *slot_no2s(int no) {
	return ("SWITCH");
}

#ifdef __cplusplus
}
#endif

#endif
