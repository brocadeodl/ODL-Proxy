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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DCMCLIENTINTERFACE_H
#define DCMCLIENTINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdarg.h"
#include "stdint.h"

typedef enum
{
    VCS_CLUSTER_TYPE_STANDALONE = 0,
    VCS_CLUSTER_TYPE_FABRIC,
    VCS_CLUSTER_TYPE_MGMT,
	VCS_CLUSTER_TYPE_INVALID 
}cluster_type;
    
typedef int (*DcmClientMessageHandler) (const void *p_input, void ** const p_output);

void add_client_message_handler (const unsigned int operation_code, DcmClientMessageHandler message_handler);

typedef int (*DcmClientPostToMainThread) (void *);

void dcm_client_set_post_to_main_thread_function (DcmClientPostToMainThread pDcmClientPostToMainThread);

void dcm_client_main_thread_posted_message_handler (void *pMessage);

/* Client initialization related API */

int dcm_initialize_client (const char *p_client_name);
int dcm_reconnect_client (void);

int dcm_uninitialize_client ();

int dcm_initialize_client_for_dual_daemons (const char *p_client_name);

int dcm_uninitialize_client_for_dual_daemons ();
/* Interface Related API for Demo Purposes Only */

/* int query_interface (const char *p_name, val_interface *p_val_interface); */

/* Physical Interface Related API */

int create_physical_interface (const char *p_name, const unsigned int if_index, const unsigned int mtu, const char *p_description);

int create_phy_interface (const char *p_name, const unsigned int if_index, const unsigned int mtu, const char *p_description, const unsigned int isl_capable);

int dcm_create_fcoe_interface (const char *if_name_p,
                               const unsigned int if_index, 
                               const char *bind_name_p,
                               const unsigned int bind_type,
                               const unsigned int fc_uport_num, 
                               const unsigned int vn_num, 
                               const unsigned int port_num,
                               const char *p_description);

int dcm_create_fcoe_interface_bulk (const void *msg_p,
                                    const unsigned int size);

int dcm_delete_fcoe_interface_bulk (const void *msg_p,
                                    const unsigned int size);

int dcm_create_portcfg_interface (const char *if_name_p, const unsigned int fc_uport_num);

int dcm_create_fcoe_et_interface (const char *if_name_p,
                                  const unsigned int if_index,
                                  const unsigned int fc_uport_num);

int dcm_create_fcoe_et_interface_bulk (const void *msg_p,
                                       const unsigned int size);

int dcm_update_fcoe_et_interface_bulk (const void *msg_p,
                                       const unsigned int size,
                                       const unsigned int oper);

int dcm_nsm_event_notify(const unsigned int eventid, const unsigned int parameter);
int dcm_nsm_notify_portrole_change (const char *p_name, unsigned int port_role, unsigned int exception, unsigned int msg_id);
int dcm_nsm_notify_phystatus_change (const char *p_name, const unsigned int flag, unsigned int msg_id);
int dcm_nsm_notify_postatus_change (const char *po_name, const unsigned int flag);
int dcm_nsm_notify_slot_offline (const unsigned int cmdcode, const unsigned int slotid);
int dcm_modify_interface_config (void *intfConfig);
int dcm_vnm_ampp_cdp_notify(const unsigned int cmdcode, const char *ifname,
                    const char *host_mor, const char *port_id, const char *vc);
int dcm_vnm_vc_cdp_notify(const unsigned int cmdcode, const char *ifname,
                    const char *host_mor, const char *port_id, const char *vc);

int create_vlan_interface (const char *p_name);

int create_po_interface (const char *p_name);

int set_vlan_desc (const char *p_name,const char *p_desc);

int set_vlan_protocalstatus (const char *p_name,const int *p_flag);

int get_vcs_id ();

int get_vcs_mode ();

int get_vcs_rbridge_id ();

int is_node_in_cluster (const unsigned int rbrigdeid);

int is_local_node_in_cluster ();

int test_client_for_fc_mode ();

int dcm_get_configuration_number (uint16_t configuration_number[16]);

int dcm_get_configuration_time (char *p_configurationTime, const int configurationTimeStringSize);

cluster_type get_this_node_cluster_type ();

int is_configfile_allowed ();

int is_cli_allowed ();

/* This API is a dummy implementaion and currently it always return 0 */
int is_gvlan_enabled ();

/* This API return 1 if the switch is in cluster mode and if the BD is mercury/castor, else it return 0 */
int is_gvlan_capable ();

int dcm_debug_vprintf (const char *p_string, va_list variableArguments);

int dcm_debug_printf (const char *p_string, ...);

int dcm_acquire_cluster_lock (const char* serviceString);

int dcm_release_cluster_lock (const char* serviceString);

int dcm_debug_infrastructure_run_script (const char *p_scriptFilename, const char *p_nosOrBash, const char *p_outputFileName, const char *p_rbridgeIdRange);

/* Config File Management */
int copy_to_startup_config (const char *path);
int copy_running_to_startup(void);
int copy_default_to_startup(void);

void dcm_set_server_port (const int serverPort);

/* legacy FOS CLI interfaces */
int fos_cli_port_persistent(const unsigned int slot, const unsigned int port, const unsigned int persistentstate);
int fos_cli_port_trunk(const unsigned int slot, const unsigned int port, const unsigned int trunkstate);

/* interface to allow maintaining PoD/DPoD license information in DCMd database */
int pod_update_license_data(const char *setName, const int len, const char *configString);

/* Request from NSM to the global plug-in to set the domain id of the RBridge that will hold the primry link for the vLAG */
/* If request is for static vLAG, the partner_key, partner_priority, and *p_partner_system will be zero */
/* Function called by NSM to request the Global plug-in to set the domain id of the RBridge */
/* that will hold the primary link for the vLAG */
/* If request is for static vLAG, the partner_key, partner_priority, and *p_partner_system will be zero */
int set_primary_link_domain_id (const char *p_port_channel_name, const unsigned int primary_link_domain_id,
                                const unsigned int partner_key, const unsigned int partner_priority,
                                const char *p_partner_system, const unsigned int command_code, unsigned int *reply_code);


int is_fabric_distribution_service_enabled ();

void dcm_vcs_fabric_debug_client_callback (uint32_t status, const void *p_output, void *p_input);

// ConfigSnapshotManagement
int copy_snapshot_from_ftp ( const char *snapshotId, unsigned int rbridgeId, const char *localFilename, const char *globalFilename);

int dcm_ag_create_fcoe_interface_bulk (void *msg_p);
int dcm_ag_delete_fcoe_interface_bulk (void *msg_p);

int dcm_create_sticky_mac_bulk (const void *msg_p, const unsigned int size);

/* Request DCM to trigger VLAN unprovisioning */
int dcm_nsm_trigger_vlan_unprovision();

// Bulking 
int load_bulking_config_file (const char *configFileName);

#ifdef __cplusplus
}
#endif

#endif /*  DCMCLIENTINTERFACE_H */
