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

/*
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 *    Module name: fabha_db_sys.h
 *
 *    Description:
 *    	Kernel support for HA mirrored database.
 */

#ifndef __SYS_FABHA_DB_SYS_H__
#define __SYS_FABHA_DB_SYS_H__

/*
 * Kernel space HA database object ADT support
 */
#if 0
#include <linux/types.h>	/* u_int */
#include <sys/fabos/cdefs.h>
#include <sys/fabos/fabha_db.h>

OS_CDECLS_BEGIN
#endif
/*
 * Tree node definitions:
 */
/* Current limits: 15 child types, 254 instances of each child type */
#define FAB_EMPTY_INST		0
#define FAB_MIN_INST		1
#define FAB_MAX_INST		254
#define FAB_ROOT_INST		1
/* 
 * A tree node can have zero child types, but the minimum valid
 * child type is 1.
 */
#define FAB_MIN_CHILD_TYPE	1
#define FAB_MAX_CHILD_TYPES	16
#define FAB_ROOT_TYPE		1

#define FAB_TNODE_MAGIC1  0x1deadce0
#define FAB_TNODE_MAGIC2 0x2beed411

/* 
 * The my_type and instance fields must be values used at the 
 * parent node to get to this node.  So, it must be true:
 * node.parent->child[node.my_type][node.instance] == &node
 *
 * The allowed_children field holds the maximum number of children that
 * this node can have.  It is set when the node is created and
 * checked as children are added.  The current_children field holds
 * the current number of children that have benn attached to this
 * node and is incremented/decremented as children are created/removed.
 */
typedef struct fab_tnode_s {
	u_int		magic;
	u_char		my_type;
	u_char		my_inst;
	unsigned short	byte_size;
	u_int		num_child_types;
	u_int		allowed_children;
	u_int		current_children;
	void		* objp;
	struct fab_tnode_s	* parentp;
	u_int		inst_count[FAB_MAX_CHILD_TYPES];
	struct fab_tnode_s	** childp[FAB_MAX_CHILD_TYPES];
	struct fab_tnode_s	* instp[1];
} fab_tnode_t;


#define FAB_TNODE_P_MAGIC(n) ((n)->magic)
#define FAB_TNODE_P_SIZE(n) ((n)->byte_size)
#define FAB_TNODE_P_TYPE(n) ((n)->my_type)
#define FAB_TNODE_P_INST(n) ((n)->my_inst)
#define FAB_TNODE_P_NUM_CHILD_TYPES(n) ((n)->num_child_types)
#define FAB_TNODE_P_ALLOWED_CHILDREN(n) ((n)->allowed_children)
#define FAB_TNODE_P_CURRENT_CHILDREN(n) ((n)->current_children)
#define FAB_TNODE_P_OBJECT(n) ((n)->objp)
#define FAB_TNODE_P_PARENT(n) ((n)->parentp)
#define FAB_TNODE_P_INST_COUNT(n, chtype) \
	((u_char)((chtype) - 1) < FAB_MAX_CHILD_TYPES ? \
		((n)->inst_count[(chtype) - 1]) : 0)
#define FAB_TNODE_P_SET_INST_COUNT(n, chtype, c) \
	if ((u_char)((chtype) - 1) < FAB_MAX_CHILD_TYPES) \
		(n)->inst_count[(chtype) - 1] = c;
#define FAB_TNODE_P_FIRST_INST_ADDR(n) (&((n)->instp[0]))
#define FAB_TNODE_P_CHILDREN(n, chtype) \
	((u_char)((chtype) - 1) < FAB_MAX_CHILD_TYPES ? \
		((n)->childp[(chtype) - 1]) : 0)
#define FAB_TNODE_P_SET_CHILDREN(n, chtype, tp) \
	if ((u_char)((chtype) - 1) < FAB_MAX_CHILD_TYPES) \
		(n)->childp[(chtype) - 1] = tp;
#define FAB_TNODE_P_CHILD(n, chtype, i) \
	((u_char)((i) - 1) < FAB_TNODE_P_INST_COUNT(n, chtype) ? \
		(n)->childp[(chtype) - 1][(i) - 1] : 0)
#define FAB_TNODE_P_SET_CHILD(n, chtype, i, ptr) \
	if ((u_char)((i) - 1) < FAB_TNODE_P_INST_COUNT(n, chtype)) \
		(n)->childp[(chtype) - 1][(i) - 1] = ptr;

extern int
fab_tnode_create(fab_tnode_t **new_node, fab_tnode_t * parentp,
		 u_char my_type, u_char inst,
		 void * localp, u_int num_child_types,
		 const u_char * child_counts);

extern int
fab_tnode_remove(fab_tnode_t *tnode);

extern const u_char fab_tnode_all_ones[];
#define FAB_TNODE_ONE_INST fab_tnode_all_ones



#endif	/* __SYS_FABHA_DB_SYS_H__ */
