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

#ifndef __NODE_ID_H__
#define __NODE_ID_H__

#ifdef __cplusplus
extern "C"
{
#endif

extern int g_thisSlotInstance;

static inline int
NID_SELF(void)
{
	return (0);
}
static inline int
NID_PEER(void)
{
	return (0);
}

static inline int
get_node_slot(int x)
{
    if ( x == 0)
        return (g_thisSlotInstance);
    else
        return (x & 0xff);
}

static inline int
get_cp_nodeip(int nodeid)
{
	return (0);
}

static inline int
get_bp_nodeip(int nodeid, int subnet)
{
	return (0);
}
static inline int
get_bp_nodeid(int slot, int inst)
{
	return (0);
}
#define NODE_STR_SIZE 32 
static inline char *
get_node_str(int node)
{
	static char str[NODE_STR_SIZE];
	strncpy(str, "node", NODE_STR_SIZE);
	return (str);
}

#ifdef __cplusplus
}
#endif

#endif // __NODE_ID_H__
