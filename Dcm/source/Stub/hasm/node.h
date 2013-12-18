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

#ifndef __SYS_NODE_H__
#define __SYS_NODE_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * once get rid of dcx support, we can use 2-bit for
 * partition instance rather than 1-bit.
 */
#define GET_NODE_SLOT_MASK  0x0000003f
#define GET_NODE_SLOT_SHIFT 0
#define GET_NODE_TYPE_MASK  0x00000f00
#define GET_NODE_TYPE_SHIFT 8
#define GET_NODE_INST_MASK  0x000000c0
#define GET_NODE_INST_SHIFT 6

/*
 * use logical slot# for cp so that its node id is platform
 * independent.
 */
#define SLOT_CP0    100
#define SLOT_CP1    101

/*
 * node type
 */
#define TYPE_CP     0   /* CP (MM) */
#define TYPE_BP     2   /* BP (LC) */

static inline int get_node_type(int x)
{
    return ((x & GET_NODE_TYPE_MASK) >> GET_NODE_TYPE_SHIFT);
}

static inline int get_node_inst(int x)
{
    return ((x & GET_NODE_INST_MASK) >> GET_NODE_INST_SHIFT);
}

#ifdef __cplusplus
}
#endif

#endif
