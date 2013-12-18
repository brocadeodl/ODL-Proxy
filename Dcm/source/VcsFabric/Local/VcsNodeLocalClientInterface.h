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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODELOCALCLIENTINTERFACE_H
#define VCSNODELOCALCLIENTINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "fc/wwn.h"
#include "sys/fabos/bitmap.h"

typedef struct
{
    unsigned int  mapped_id;
} vcs_local_node_params;



/* Interface to push FCF MAC address from FCOE daemon */
int vcs_set_local_fcf_mac_interface (const char fcf_mac[6]);

/* Interface for protocol daemons to get local mapped id */
int vcs_get_local_mapped_id_interface (vcs_local_node_params *local_params);
int vcs_get_local_mapped_id_cached_interface (vcs_local_node_params *local_params);

/* Interface for protocol daemons to get rbridge id from a given WWN */
int vcs_get_rbridge_id_interface (unsigned int *rbridge_id, const wwn_t *wwn);

/* Interface for protocol daemons to get the WWN from a given rbridge id */
int vcs_get_wwn_interface (wwn_t *wwn, const unsigned int *rbridge_id);

/* Interface for protocol daemons to get internal IP address from a given rbridge id */
int vcs_get_ip_interface (char **ip, const unsigned int *rbridge_id);

/* Interface for protocol daemons to get rbridgeId in cluster */
int vcs_get_cluster_rbridge_id_from_wwn (const wwn_t *wwn, domainBitMap_t *rbridgeIdBitmap);

#ifdef __cplusplus
}
#endif

#endif // VCSNODELOCALCLIENTINTERFACE_H 
