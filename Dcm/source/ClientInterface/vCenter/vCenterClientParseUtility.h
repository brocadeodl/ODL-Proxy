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

#ifndef VCENTERPARSEUTILITY_H
#define VCENTERPARSEUTILITY_H
#include "ClientInterface/vCenter/vCenterMessageDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IP_STRING_LEN 20
#define MAC_STRING_LEN 20
    /**
     * Gets the data of type vnm_resp_t
     */
    vnm_resp_t* vnm_get_resp(const void *in_p);

    /**
     * Extract the message id from the response
     */
    uint32_t vnm_get_msg_id(vnm_resp_t *vnm_resp);

    /**
     * Get the Response type from the Response.
     */
    vnm_rsp_type_t vnm_get_resp_type(vnm_resp_t *vnm_resp);

    /**
     * Find the number of payloads
     */
    u_int32_t vnm_get_num_of_plds(vnm_resp_t *vnm_resp);

    /**
     * Get the first notification payload from the response
     */
    vnm_mig_notif_pld_t* vnm_get_first_notif_pld(vnm_resp_t *vnm_resp);

    /**
     * Get the next notification payload given the previous payload.
     * This should be called based on the number of payloads.
     */
    vnm_mig_notif_pld_t* vnm_get_next_notif_pld(vnm_mig_notif_pld_t* prev_pld);

    /**
     * Get the VM Details in the the notification payload
     */
    vnm_vm_dtls_t* vnm_get_mig_notif_vm_dtls(vnm_mig_notif_pld_t *notif_pld);

    /**
     * Returns an array of VM IP Address Mac Address Map in the VM Details structure
     */
    vnm_mac_ip_map_t * vnm_get_mig_notif_vm_mac_ip_map_array(
            vnm_vm_dtls_t *vm_dtls);

    /**
     * Returns an array of VM Mac address in the VM Details structure
     */
    struct ether_addr * vnm_get_mig_notif_vm_mac_array(vnm_vm_dtls_t *vm_dtls);

    /**
     * Utility function to calculate the fixed size of notif payload
     */
    size_t vnm_get_mig_notif_pld_fixed_size();
    /**
     * Utility function to calculate the size of VM Details in notif payload
     */
    size_t vnm_get_mig_notif_pld_vm_dtls_size(vnm_mig_notif_pld_t *notif_pld);
    /**
     * Utility function to calculate the size of local ESX details in notif payload
     */
    size_t vnm_get_mig_notif_pld_lcl_esx_size(vnm_mig_notif_pld_t *notif_pld);
    /**
     * Utility function to calculate the size of Remote ESX details in notif payload
     */
    size_t vnm_get_mig_notif_pld_rmt_esx_size(vnm_mig_notif_pld_t *notif_pld);
    /**
     * Utility function to calculate the total size of notif payload
     */
    size_t vnm_get_mig_notif_pld_total_size(vnm_mig_notif_pld_t *notif_pld);
    /**
     * Returns Local ESX Details in a notification payload
     */
    vnm_esx_dtls_t * vnm_get_mig_notif_pld_lcl_esx_dtls(
            vnm_mig_notif_pld_t *notif_pld);
    /**
     * Returns Remote ESX Details in a notification payload
     */
    vnm_esx_dtls_t* vnm_get_mig_notif_pld_rmt_esx_dtls(
            vnm_mig_notif_pld_t *notif_pld);

    //Power Payload Access Library
    /**
     * Utility function to calculate the fixed size of power payload
     */
    size_t vnm_get_power_pld_fixed_size();
    /**
     * Utility function to calculate the VM Details size for Power payload
     */
    size_t vnm_get_power_pld_vm_dtls_size(vnm_vm_power_pld_t *pld);
    /**
     * Utility function to calculate the Power Payload total size
     */
    size_t vnm_get_power_pld_total_size(vnm_vm_power_pld_t *pld);
    /**
     * Get the first Power Payload in Response.
     */
    vnm_vm_power_pld_t* vnm_get_first_power_pld(vnm_resp_t *vnm_resp);
    /**
     * Get the next Power Payload given the earlier power payload
     */
    vnm_vm_power_pld_t* vnm_get_next_power_pld(vnm_vm_power_pld_t* prev_pld);

    /**
     * Get the VM Details in a power payload
     */
    vnm_vm_dtls_t* vnm_get_mig_power_vm_dtls(vnm_vm_power_pld_t *power_pld);

    /**
     * Utility function to calculate the size of Notif payload based on variables
     */
    size_t calculateSizeOfNotifPayload(int nu_vm_ip, int nu_vm_mac,
            int nu_src_esx_kernel_ip, int nu_dst_esx_kernel);
    /**
     * Utility funtion to calculate the size of Power payload based on variables.
     */
    size_t calculateSizeOPowerPayload(int nu_vm_ip, int nu_vm_mac);
    /**
     * Utiltity function to populate vm details
     */
    void populate_vm_dtls(vnm_vm_dtls_t *vm_dtls, uint32_t nu_vm_ips,
            uint32_t nu_vm_macs, char **vm_ip_array, char **vm_mac_array);
    /**
     * Utiltiy function to populate ESX details
     */
    void populate_esx_dtls(vnm_esx_dtls_t *esx_dtls,
            uint32_t num_of_kernel_ip_addrs, vnm_esx_role_t esx_role,
            char *console_ip, char **kern_ip_array);
    /**
     * Utility function to allocate memory for power payloads
     * Populate size field to the allocated memory
     */
    vnm_resp_t * allocate_memory_for_power_plds(int number_of_plds,
            int nu_vm_ip, int nu_vm_mac);
    /**
     * Utility function to allocate memory for notif payloads.
     * Populate size field to the allocated memory
     */
    vnm_resp_t * allocate_memory_for_notif_plds(int number_of_plds,
            int nu_vm_ip, int nu_vm_mac, int nu_lcl_esx_ker_ip,
            int nu_rmt_esx_ker_ip);

    int convert_string_to_ip(struct vnm_ip_addr_t *addr, const char *ip);

    char * convert_ip_to_string(struct vnm_ip_addr_t addr);

    bool vnm_is_nicType_Management(uint32_t nicType);
    bool vnm_is_nicType_vMotion(uint32_t nicType);
    bool vnm_is_nicType_faultTolerance(uint32_t nicType);

#ifdef __cplusplus
}
#endif

#endif /* VCENTERPARSEUTILITY_H */

