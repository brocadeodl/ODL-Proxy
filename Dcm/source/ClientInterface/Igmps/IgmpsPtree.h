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

#ifndef _IGMPS_PTREE_H
#define _IGMPS_PTREE_H 

namespace DcmNs
{
/* Patricia tree top structure. */
struct ptree
{
	/* Top node. */
	struct ptree_node *top;

	/* Maximum key size allowed (in bits). */
	unsigned int max_key_len;
};

/* Patricia tree node structure. */
struct ptree_node
{
  struct ptree_node *link[2];
#define  p_left      link[0]
#define  p_right     link[1]

  /* Tree link. */
  struct ptree *tree;
  struct ptree_node *parent;

  /* Lock of this radix. */
  unsigned int lock;

  /* Each node of route. */
  void *info;
 
  /* Key len (in bits). */
  unsigned int key_len;

  /* Key begins here. */
  unsigned char key [1];
};

#define PTREE_KEY_MIN_LEN       1
#define PTREE_NODE_KEY(n)       (& (n)->key [0])

#define PTREE_LOOP(T,V,N)                                 \
  if (T)                                                  \
    for ((N) = ptree_top ((T)); (N); (N) = ptree_next ((N)))  \
      if (((V) = (N)->info) != NULL)

/* Prototypes. */
struct ptree *ptree_init (unsigned int max_key_len);
struct ptree_node *ptree_top (struct ptree *tree);
struct ptree_node *ptree_next (struct ptree_node *node);
struct ptree_node *ptree_next_until (struct ptree_node *node1,
				     struct ptree_node *node2);
struct ptree_node *ptree_node_get (struct ptree *tree, unsigned char *key,
				   unsigned int key_len);
struct ptree_node *ptree_node_lookup (struct ptree *tree, unsigned char *key,
				      unsigned short key_len);
struct ptree_node *ptree_lock_node (struct ptree_node *node);
struct ptree_node *ptree_node_match (struct ptree *tree, unsigned char *key,
				     unsigned short key_len);
void ptree_node_free (struct ptree_node *node);
void ptree_finish (struct ptree *tree);
void ptree_unlock_node (struct ptree_node *node);
void ptree_node_delete (struct ptree_node *node);
void ptree_node_delete_all (struct ptree *tree);
int ptree_has_info (struct ptree *tree);
void
ptree_key_copy (struct ptree_node *node, unsigned char *key, unsigned short key_len);
int
ptree_bit_to_octets (unsigned short key_len);
int
ptree_key_match (unsigned char *np, unsigned short n_len, unsigned char *pp, unsigned short p_len);
int
ptree_check_bit (struct ptree *tree, unsigned char *p, unsigned short key_len);

}
/* End Prototypes. */

#endif 
