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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#ifndef __VCENTER_MESSAGE_DEF_H__
#define __VCENTER_MESSAGE_DEF_H__

#include <netinet/in.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netdb.h>
//#define VCENTEREVENTS         23201
#define POWER_PLD_REQ_TAG     32
#define POWER_PLD_RESP_TAG    33
#define NOTIFICATION_PLD_TAG    34
#define VNM_VM_ID_MAX_LEN 20
#define VNM_ETHER_ADDR_MAX_LEN 20


#pragma pack(push,1)
//#pragma pack(1)
 struct vnm_ip_addr_t {
        sa_family_t family;
        union {
                struct in6_addr in6addr;
                struct in_addr in4addr;
        } ipaddr;
};

typedef enum vnm_rsp_type_ {
    VNM_RSP_MIG_DTLS, // Response for Migration details between pair of ESX
    VNM_RSP_VM_MIG_STATUS, // Response for Migration status of a VM
    VNM_RSP_VM_POWER_STATUS, // Response for Power status of a VM
    VNM_NOTIF_MIG_DTLS,
    VNM_RSP_FAILURE
// Notification Response.
} vnm_rsp_type_t;

typedef struct vnm_rsp_hdr_ {
        uint32_t req_msg_id;
} vnm_rsp_hdr_t;

typedef enum vnm_esx_role_ {
    VNM_ESX_SOURCE,
    VNM_ESX_DESTINATION
} vnm_esx_role_t;

typedef struct vnm_esx_kernel_ip_{
        uint32_t nic_type;
        struct vnm_ip_addr_t ip;
}vnm_esx_kernel_ip_t;

typedef struct vnm_esx_dtls_ {
        uint32_t num_of_kernel_ip_addrs; //ESX kernel IP address count
        vnm_esx_role_t esx_role;
        struct vnm_ip_addr_t esx_ip_addr; //console address
        vnm_esx_kernel_ip_t esx_kernel_ip_addrs[1];

} vnm_esx_dtls_t;

typedef enum vnm_vm_migration_status_ {
    VNM_VM_MIGRATION_INPROGRESS,
    VNM_VM_MIGRATION_COMPLETED,
    VNM_VM_MIGRATION_FAILED
} vnm_vm_migration_status_t;

/*
typedef struct ether_addr_ {
        char addr[VNM_ETHER_ADDR_MAX_LEN];
} ether_addr_t; */

typedef struct vnm_mac_ip_map_{
        struct vnm_ip_addr_t vm_ip_addr;
        struct ether_addr vm_mac;
}vnm_mac_ip_map_t;

typedef struct vnm_vm_dtls_ {
        uint32_t num_of_mac_ip_maps;
        uint32_t num_of_vm_macs;
        vnm_mac_ip_map_t vnm_mac_ip_maps[1];
        struct ether_addr vm_macs[1];

} vnm_vm_dtls_t;

typedef enum vnm_err_code_ {
    VNM_ERR_SUCCESS,
    VNM_ERR_FAILURE,
    VNM_ERR_VMOTION_MANAGER_NO_CONNECTIVITY,
    VNM_ERR_VMOTION_MANAGER_INVALID_CONNECTION,
    VNM_ERR_VMOTION_MANAGER_INVALID_CREDENTIALS,
    VNM_ERR_VMOTION_MANAGER_INVALID_VERSION,
    VNM_ERR_VMOTION_MANAGER_NO_SERVICE,
    VNM_ERR_VMOTION_MANAGER_NOT_CONFIGURED_ACTIVATED,
    VNM_ERR_VMOTION_MANAGER_INVALID_VM,
    VNM_ERR_VMOTION_CAPABILITY_DISABLED,
    VNM_ERR_OPERATION_NOT_SUPPORTED,
    VNM_ERR_DCMCLIENT_CONNECTIVITY_ERROR,
    VNM_ERR_UNKNOWN
} vnm_err_code_t;

typedef enum vnm_vm_power_status_ {
    VNM_VM_POWERED_OFF,
    VMM_VM_POWERED_ON,
    VNM_VM_SUSPENDED
} vnm_vm_power_status_t;

typedef struct vnm_mig_cookie {
        struct vnm_ip_addr_t vcenter_ip;
        uint32_t mig_task_id;
        char vm_id[VNM_VM_ID_MAX_LEN];
} vnm_mig_cookie_t;

typedef struct vnm_vm_power_pld_ {
        vnm_err_code_t ret_code;
        vnm_mig_cookie_t cookie;
        vnm_vm_power_status_t vnm_power_status;
        vnm_vm_dtls_t vm_dtls;

} vnm_vm_power_pld_t;

typedef struct vnm_vm_mig_pld_ {
        vnm_err_code_t ret_code;
        vnm_mig_cookie_t cookie;
        vnm_vm_migration_status_t vm_mig_status;
        vnm_vm_dtls_t vm_dtls;
} vnm_vm_mig_pld_t;
typedef struct vnm_mig_dtls_pld_ {
        vnm_err_code_t ret_code;
        vnm_mig_cookie_t cookie;
        vnm_vm_migration_status_t vm_migration_status;
        vnm_vm_dtls_t vm_dtls;
        vnm_esx_dtls_t lcl_esx_dtls; //ESX details including the role
        vnm_esx_dtls_t rmt_esx_dtls;

} vnm_mig_dtls_pld_t, vnm_mig_notif_pld_t;

typedef struct vnm_rsp_msg_data_ {

        u_int32_t num_plds; //number of payloads
        union { //payloads populated based on the type
                vnm_mig_dtls_pld_t mig_rsp_pld[1];
                vnm_vm_mig_pld_t vm_mig_pld[1];
                vnm_vm_power_pld_t vm_pwd_pld[1];
                vnm_mig_notif_pld_t mig_notif_pld[1];

        } payload;
} vnm_rsp_msg_data_t;

typedef struct vnm_resp_ {
        size_t size;
        vnm_rsp_hdr_t rsp_hdr;
        vnm_rsp_type_t rsp_type; //Indicates the type (response-message or notification)
        vnm_rsp_msg_data_t msg;
} vnm_resp_t;

//Request Structure
typedef enum vnm_req_type_ {
    VNM_REQ_MIG_DTLS, //Request for Migration details between pair of ESX
    VNM_REQ_VM_MIG_STATUS, //Request for Migration status of a VM
    VNM_REQ_VM_MIG_STATUS_USING_COOKIE, //Request for Migration status of a VM using cookie
    VNM_REQ_VM_POWER_STATUS, // Request for Power Status of a VM
    VNM_REQ_VM_POWER_STATUS_USING_COOKIE

} vnm_req_type_t;

typedef struct vnm_esx_pair_ {
        struct vnm_ip_addr_t lcl_site_esx_ip_addr;
        struct vnm_ip_addr_t rmt_site_esx_ip_addr;
} vnm_esx_pair_t;


typedef struct vnm_req_ {
        vnm_req_type_t req_type;
        u_int32_t num_plds;
        union  {
                vnm_esx_pair_t esx_pair[1];
                struct vnm_ip_addr_t vm_ip_addr[1];
                vnm_mig_cookie_t cookie[1];
        }payload;
} vnm_req_t;
#pragma pack(pop)
#endif    /* __VCENTER_MESSAGE_DEF_H__ */
