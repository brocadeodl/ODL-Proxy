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

#include <iostream>
#include "Framework/Core/Wave.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ClientInterface/vCenter/vCenterClientParseUtility.h"
#include <netinet/ether.h>
using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C" {
    vnm_resp_t* vnm_get_resp(const void *in_p) {
        vnm_resp_t *vnm_resp = (vnm_resp_t *) in_p;
        return vnm_resp;
    }

    uint32_t vnm_get_msg_id(vnm_resp_t *vnm_resp) {
        return vnm_resp->rsp_hdr.req_msg_id;
    }

    vnm_rsp_type_t vnm_get_resp_type(vnm_resp_t *vnm_resp) {
        return vnm_resp->rsp_type;
    }

    u_int32_t vnm_get_num_of_plds(vnm_resp_t *vnm_resp) {
        return vnm_resp->msg.num_plds;
    }

    bool vnm_is_nicType_Management(uint32_t nicType) {
        return nicType & 2;
    }
    bool vnm_is_nicType_vMotion(uint32_t nicType) {
        return nicType & 4;
    }
    bool vnm_is_nicType_faultTolerance(uint32_t nicType) {
        return nicType & 1;
    }

    vnm_mig_notif_pld_t* vnm_get_first_notif_pld(vnm_resp_t *vnm_resp) {

        printf(
                "Payload offset %ld\n",
                (long int)((char *) vnm_resp->msg.payload.mig_notif_pld
                - (char *) vnm_resp));
        return vnm_resp->msg.payload.mig_notif_pld;

    }

    vnm_mig_notif_pld_t* vnm_get_next_notif_pld(vnm_mig_notif_pld_t* prev_pld) {
        char * ptr = (char *) prev_pld;
        ptr += vnm_get_mig_notif_pld_total_size(prev_pld);

        printf("Next Payload offset %ld\n",
                (long int)vnm_get_mig_notif_pld_total_size(prev_pld));


        return (vnm_mig_notif_pld_t *) ptr;
    }

    vnm_vm_dtls_t* vnm_get_mig_notif_vm_dtls(vnm_mig_notif_pld_t *notif_pld) {
        char *ptr = (char *) notif_pld;
        vnm_vm_dtls_t *vnm_vm_dtls = (vnm_vm_dtls_t*) (ptr
                + vnm_get_mig_notif_pld_fixed_size());
        printf("VM Details offset %ld\n", (long int)((char *) vnm_vm_dtls - ptr));
        return vnm_vm_dtls;
    }

    vnm_mac_ip_map_t * vnm_get_mig_notif_vm_mac_ip_map_array(vnm_vm_dtls_t *vm_dtls) {
        char *ptr = (char *) vm_dtls;
         vnm_mac_ip_map_t *vm_mac_ip = ( vnm_mac_ip_map_t*) (ptr + sizeof(uint32_t)
                + sizeof(uint32_t));
        return vm_mac_ip;
    }

    struct ether_addr * vnm_get_mig_notif_vm_mac_array(vnm_vm_dtls_t *vm_dtls) {
        char *ptr = (char *) vm_dtls;
        uint32_t num_of_mac_ip_maps = vm_dtls->num_of_mac_ip_maps;
        uint32_t multiplier_num_of_mac_ip_maps = num_of_mac_ip_maps == 0 ? 1 : num_of_mac_ip_maps;
        struct ether_addr *vm_mac_addrs = (struct ether_addr*) (ptr + sizeof(uint32_t)
                + sizeof(uint32_t) + multiplier_num_of_mac_ip_maps * sizeof( vnm_mac_ip_map_t));
        return vm_mac_addrs;
    }

    size_t vnm_get_mig_notif_pld_fixed_size() {
        return sizeof(vnm_vm_migration_status_t) + sizeof(vnm_mig_cookie_t)
                + sizeof(vnm_err_code_t);
    }

    size_t vnm_get_mig_notif_pld_vm_dtls_size(vnm_mig_notif_pld_t *notif_pld) {
        uint32_t num_of_mac_ip_maps = notif_pld->vm_dtls.num_of_mac_ip_maps;
        uint32_t multiplier_num_of_mac_ip_maps = num_of_mac_ip_maps == 0 ? 1 : num_of_mac_ip_maps;

        uint32_t nu_of_mac = notif_pld->vm_dtls.num_of_vm_macs;
        uint32_t multiplier_mac = nu_of_mac == 0 ? 1 : nu_of_mac;
        size_t vm_dtls_size = sizeof(uint32_t) + sizeof(uint32_t)
                + +multiplier_num_of_mac_ip_maps * sizeof( vnm_mac_ip_map_t)
                + multiplier_mac * sizeof(struct ether_addr);
        return vm_dtls_size;

    }
    size_t vnm_get_mig_notif_pld_lcl_esx_size(vnm_mig_notif_pld_t *notif_pld) {
        vnm_esx_dtls_t *src_esx_dtls = vnm_get_mig_notif_pld_lcl_esx_dtls(
                notif_pld);

        uint32_t num_of_kernel_ip_addrs = src_esx_dtls->num_of_kernel_ip_addrs;
        uint32_t kernel_ip_addr_mult =
                num_of_kernel_ip_addrs == 0 ? 1 : num_of_kernel_ip_addrs;
        printf("Kernel IP %d", ( int)num_of_kernel_ip_addrs);

        size_t src_esx_dtls_size = sizeof(num_of_kernel_ip_addrs)
                + sizeof(vnm_esx_role_t) + sizeof(struct vnm_ip_addr_t)
                + kernel_ip_addr_mult * sizeof( vnm_esx_kernel_ip_t);

        return src_esx_dtls_size;
    }

    size_t vnm_get_mig_notif_pld_rmt_esx_size(vnm_mig_notif_pld_t *notif_pld) {
        vnm_esx_dtls_t *dst_esx_dtls = vnm_get_mig_notif_pld_rmt_esx_dtls(
                notif_pld);

        uint32_t num_of_kernel_ip_addrs = dst_esx_dtls->num_of_kernel_ip_addrs;
        uint32_t kernel_ip_addr_mult =
                num_of_kernel_ip_addrs == 0 ? 1 : num_of_kernel_ip_addrs;

        size_t src_esx_dtls_size = sizeof(num_of_kernel_ip_addrs)
                + sizeof(vnm_esx_role_t) + sizeof(struct vnm_ip_addr_t)
                + kernel_ip_addr_mult * sizeof( vnm_esx_kernel_ip_t);

        return src_esx_dtls_size;
    }

    size_t vnm_get_mig_notif_pld_total_size(vnm_mig_notif_pld_t *notif_pld) {
        return vnm_get_mig_notif_pld_fixed_size()
                + vnm_get_mig_notif_pld_vm_dtls_size(notif_pld)
                + vnm_get_mig_notif_pld_lcl_esx_size(notif_pld)
                + vnm_get_mig_notif_pld_rmt_esx_size(notif_pld);

    }

    vnm_esx_dtls_t * vnm_get_mig_notif_pld_lcl_esx_dtls(
            vnm_mig_notif_pld_t *notif_pld) {
        char *ptr = (char *) notif_pld;
        vnm_esx_dtls_t *vnm_src_esx_dtls = (vnm_esx_dtls_t*) (ptr
                + vnm_get_mig_notif_pld_fixed_size()
                + vnm_get_mig_notif_pld_vm_dtls_size(notif_pld));
        printf("LCL ESX Array offset %ld\n", (long int)((char *) vnm_src_esx_dtls - ptr));

        return vnm_src_esx_dtls;
    }
    vnm_esx_dtls_t* vnm_get_mig_notif_pld_rmt_esx_dtls(
            vnm_mig_notif_pld_t *notif_pld) {
        char *ptr = (char *) notif_pld;
        vnm_esx_dtls_t *vnm_dst_esx_dtls = (vnm_esx_dtls_t*) (ptr
                + vnm_get_mig_notif_pld_fixed_size()
                + vnm_get_mig_notif_pld_vm_dtls_size(notif_pld)
                + vnm_get_mig_notif_pld_lcl_esx_size(notif_pld));
        printf("RMT ESX Array offset %ld\n", (long int)((char *) vnm_dst_esx_dtls - ptr));
        return vnm_dst_esx_dtls;
    }

    //Power Payload Access Library

    size_t vnm_get_power_pld_fixed_size() {
        return sizeof(vnm_vm_power_status_t) + sizeof(vnm_mig_cookie_t)
                + sizeof(vnm_err_code_t);
    }
    size_t vnm_get_power_pld_vm_dtls_size(vnm_vm_power_pld_t *pld) {
        uint32_t num_of_mac_ip_maps = pld->vm_dtls.num_of_mac_ip_maps;
        uint32_t multiplier_num_of_mac_ip_maps = num_of_mac_ip_maps == 0 ? 1 : num_of_mac_ip_maps;

        uint32_t nu_of_mac = pld->vm_dtls.num_of_vm_macs;
        uint32_t multiplier_mac = nu_of_mac == 0 ? 1 : nu_of_mac;
        size_t vm_dtls_size = sizeof(uint32_t)
                + multiplier_num_of_mac_ip_maps * sizeof( vnm_mac_ip_map_t)
                + multiplier_mac * sizeof(struct ether_addr);
        return vm_dtls_size;

    }
    size_t vnm_get_power_pld_total_size(vnm_vm_power_pld_t *pld) {
        return vnm_get_power_pld_fixed_size()
                + vnm_get_power_pld_vm_dtls_size(pld);

    }
    vnm_vm_power_pld_t* vnm_get_first_power_pld(vnm_resp_t *vnm_resp) {

        return vnm_resp->msg.payload.vm_pwd_pld;

    }
    vnm_vm_power_pld_t* vnm_get_next_power_pld(vnm_vm_power_pld_t* prev_pld) {
        char * ptr = (char *) prev_pld;
        ptr += vnm_get_power_pld_total_size(prev_pld);
        return (vnm_vm_power_pld_t *) ptr;
    }

    vnm_vm_dtls_t* vnm_get_mig_power_vm_dtls(vnm_vm_power_pld_t *notif_pld) {
        char *ptr = (char *) notif_pld;
        vnm_vm_dtls_t *vnm_vm_dtls = (vnm_vm_dtls_t*) (ptr
                + vnm_get_power_pld_fixed_size());
        return vnm_vm_dtls;
    }

    size_t calculateSizeOfNotifPayload(int nu_vm_mac_ip_map, int nu_vm_mac,
            int nu_src_esx_kernel_ip, int nu_dst_esx_kernel) {
        size_t HEADER_OF_PAYLOAD = vnm_get_mig_notif_pld_fixed_size();
        printf("HP %ld\n",(long int)HEADER_OF_PAYLOAD);

        //vnm_vm_dtls_size  = num_of_ip_addrs + nu_vm_ip  * vm_ip_addrs + num_of_macs + num_vm_mac * vm_mac_addrs
        size_t vnm_vm_dtls_size = sizeof(uint32_t)
                + nu_vm_mac_ip_map * sizeof( vnm_mac_ip_map_t) + sizeof(uint32_t)
                + nu_vm_mac * sizeof(struct ether_addr);
        printf("vnm_vm_dtls_size %ld\n",(long int)vnm_vm_dtls_size);

        //vnm_esx_dtls_size  = num_of_kernel_ip_addrs + vnm_esx_role_t  + esx_ip_addr + nu_src_esx_kernel_ip* esx_kernel_ip_addrs
        size_t src_esx_dtls_size = sizeof(uint32_t) + sizeof(vnm_esx_role_t)
                + sizeof(struct vnm_ip_addr_t)
                + nu_src_esx_kernel_ip * sizeof( vnm_esx_kernel_ip_t);
        printf("src_esx_dtls_size %ld\n",(long int)src_esx_dtls_size);

        size_t dst_esx_dtls_size = sizeof(uint32_t) + sizeof(vnm_esx_role_t)
                + sizeof(struct vnm_ip_addr_t)
                + nu_dst_esx_kernel * sizeof( vnm_esx_kernel_ip_t);
        printf("dst_esx_dtls_size %ld\n",(long int)dst_esx_dtls_size);

        size_t payload_size = HEADER_OF_PAYLOAD + vnm_vm_dtls_size
                + src_esx_dtls_size + dst_esx_dtls_size;
        printf("payload_size %ld\n",(long int)payload_size);

        return payload_size;
    }

    size_t calculateSizeOPowerPayload(int nu_vm_ip, int nu_vm_mac) {
        size_t HEADER_OF_PAYLOAD = vnm_get_power_pld_fixed_size();

        //vnm_vm_dtls_size  = num_of_ip_addrs + nu_vm_ip  * vm_ip_addrs + num_of_macs + num_vm_mac * vm_mac_addrs
        size_t vnm_vm_dtls_size = sizeof(uint32_t)
                + nu_vm_ip * sizeof(struct vnm_ip_addr_t) + sizeof(uint32_t)
                + nu_vm_mac * sizeof(struct ether_addr);

        size_t payload_size = HEADER_OF_PAYLOAD + vnm_vm_dtls_size;

        return payload_size;
    }

   /* void populate_vm_dtls(vnm_vm_dtls_t *vm_dtls, uint32_t nu_vm_ips,
            uint32_t nu_vm_macs, char **vm_ip_array, char **vm_mac_array) {
        //TODO externalize the size of IP & Mac array
        vm_dtls->num_of_ip_addrs = nu_vm_ips;
        vm_dtls->num_of_vm_macs = nu_vm_macs;
        struct vnm_ip_addr_t * vm_dtls_array = vnm_get_mig_notif_vm_ip_array(vm_dtls);
        uint32_t i = 0;
        char *ip_ptr = (char *) vm_ip_array;
        for (i = 0; i < nu_vm_ips; i++, ip_ptr += IP_STRING_LEN) {
            char ip[IP_STRING_LEN];
            strncpy(ip, ip_ptr, IP_STRING_LEN);

            struct vnm_ip_addr_t vmIP;
            convert_string_to_ip( &vmIP,ip);

            vm_dtls_array[i] = vmIP;
        }

        struct ether_addr * vm_dtls_mac_array = vnm_get_mig_notif_vm_mac_array(
                vm_dtls);
        char *mac_ptr = (char *) vm_mac_array;

        for (i = 0; i < nu_vm_macs; i++, mac_ptr += MAC_STRING_LEN) {
            struct ether_addr ether;
            struct ether_addr * s =
                                    (struct ether_addr *)ether_aton(mac_ptr);
            memcpy(&ether, s, sizeof(struct ether_addr));

            vm_dtls_mac_array[i] = ether;
        }

    }

    void populate_esx_dtls(vnm_esx_dtls_t *esx_dtls,
            uint32_t num_of_kernel_ip_addrs, vnm_esx_role_t esx_role,
            char *console_ip, char **kern_ip_array) {
        //TODO externalize the size of IP array
        esx_dtls->num_of_kernel_ip_addrs = num_of_kernel_ip_addrs;
        esx_dtls->esx_role = esx_role;
        struct vnm_ip_addr_t conIP;
        convert_string_to_ip( &conIP,console_ip);
        esx_dtls->esx_ip_addr = conIP;

        uint32_t i = 0;
        char *ip_ptr = (char *) kern_ip_array;
        for (i = 0; i < num_of_kernel_ip_addrs; i++, ip_ptr += IP_STRING_LEN) {
            char ip[IP_STRING_LEN];
            strncpy(ip, ip_ptr, IP_STRING_LEN);

            struct vnm_ip_addr_t kernelIP;
         ;
            convert_string_to_ip( &kernelIP,ip);
            esx_dtls->esx_kernel_ip_addrs[i].ip = kernelIP;
        }
    }*/
    vnm_resp_t * allocate_memory_for_power_plds(int number_of_plds,
            int nu_vm_ip, int nu_vm_mac) {
        //TODO Remove printfs
        //header_size = vnm_rsp_hdr_t + vnm_rsp_type_t
        size_t header_size = sizeof(size_t) + sizeof(vnm_rsp_hdr_t)
                + sizeof(vnm_rsp_type_t);

        //payload_size = num_plds + NUMBER_OF_NOTIF_PAYLOADS * vnm_mig_notif_pld_t
        size_t payload_size = sizeof(u_int32_t)
                + number_of_plds
                        * calculateSizeOPowerPayload(nu_vm_ip, nu_vm_mac);

        size_t TOTAL_SIZE = header_size + payload_size;

        printf("Size allocated %ld\n", (long int)TOTAL_SIZE);
        printf(
                "Size Normal %ld\n",
                (long int)(sizeof(vnm_vm_power_pld_t) + sizeof(u_int32_t)
                        + sizeof(vnm_rsp_hdr_t) + sizeof(vnm_rsp_type_t)));

        //Creating VNM_RESP of Notification Type
        vnm_resp_t *resp = (vnm_resp_t*) calloc(1, TOTAL_SIZE);
        resp->size = TOTAL_SIZE;
        return resp;

    }
    vnm_resp_t * allocate_memory_for_notif_plds(int number_of_plds,
            int num_of_mac_ip_maps, int nu_vm_mac, int nu_lcl_esx_ker_ip,
            int nu_rmt_esx_ker_ip) {
        int number_of_plds_def = (number_of_plds ==0)?1:number_of_plds;
        int nu_vm_ip_def = (num_of_mac_ip_maps ==0)?1:num_of_mac_ip_maps;
        int nu_vm_mac_def = (nu_vm_mac ==0)?1:nu_vm_mac;
        int nu_lcl_esx_ker_ip_def = (nu_lcl_esx_ker_ip ==0)?1:nu_lcl_esx_ker_ip;
        int nu_rmt_esx_ker_ip_def = (nu_rmt_esx_ker_ip ==0)?1:nu_rmt_esx_ker_ip;


        //TODO remove printfs
        //header_size = size +  vnm_rsp_hdr_t + vnm_rsp_type_t
        size_t header_size = sizeof(size_t) + sizeof(vnm_rsp_hdr_t)
                + sizeof(vnm_rsp_type_t);

        printf("HS %ld\n",(long int)header_size);

        //payload_size = num_plds + NUMBER_OF_NOTIF_PAYLOADS * vnm_mig_notif_pld_t
        size_t payload_size = sizeof(u_int32_t)
                + number_of_plds_def
                        * calculateSizeOfNotifPayload(nu_vm_ip_def, nu_vm_mac_def,
                                nu_lcl_esx_ker_ip_def, nu_rmt_esx_ker_ip_def);

        printf("PS %ld\n",(long int)payload_size);
        size_t TOTAL_SIZE = header_size + payload_size;

        printf("Size allocated %ld\n",(long int) TOTAL_SIZE);
        printf("Original Size allocated %ld\n",(long int) sizeof(vnm_resp_t));

        //Creating VNM_RESP of Notification Type
        vnm_resp_t *resp = (vnm_resp_t*) calloc(1, TOTAL_SIZE);
        resp->size = TOTAL_SIZE;

        return resp;

    }
    int convert_string_to_ip( struct vnm_ip_addr_t *addr,const char *ip) {
        struct addrinfo hint, *res = NULL;
        int ret;

        memset(&hint, '\0', sizeof hint);

        hint.ai_family = PF_UNSPEC;
        hint.ai_flags = AI_NUMERICHOST;

        ret = getaddrinfo(ip, NULL, &hint, &res);
        if (ret) {
            return 1;
        }
        addr->family = res->ai_family;
        if (addr->family == AF_INET) {

            struct sockaddr_in * ip_soc = (struct sockaddr_in *) res->ai_addr;
            memcpy(&addr->ipaddr.in4addr, &ip_soc->sin_addr, sizeof(struct in_addr));

        }
        if (addr->family == AF_INET6) {

            struct sockaddr_in6 * ip_soc = (struct sockaddr_in6 *) res->ai_addr;
            memcpy(&addr->ipaddr.in6addr, &ip_soc->sin6_addr, sizeof(struct in6_addr));

        }
        freeaddrinfo(res);

        return 0;
    }
    char * convert_ip_to_string(struct vnm_ip_addr_t addr) {

        char *ip = (char *)calloc(1,INET6_ADDRSTRLEN);

        if (addr.family == AF_INET) {

            inet_ntop(addr.family, &(addr.ipaddr.in4addr), ip, INET6_ADDRSTRLEN);

        }
        if (addr.family == AF_INET6) {

            inet_ntop(addr.family, &(addr.ipaddr.in6addr), ip, INET6_ADDRSTRLEN);

        }

        return ip;
    }
    /*void main_notif(void) {

        //Creating VNM_RESP of Notification Type
        vnm_resp_t *resp = allocate_memory_for_notif_plds(2, 2, 2, 2, 2);

        resp->rsp_hdr.req_msg_id = 1000;
        resp->rsp_type = VNM_NOTIF_MIG_DTLS;

        resp->msg.num_plds = 1;
        resp->msg.payload.mig_notif_pld[0].cookie.mig_task_id = 1203;
        //resp->msg.payload.mig_notif_pld[0].cookie.vcenter_ip =
        resp->msg.payload.mig_notif_pld[0].ret_code = VNM_ERR_FAILURE;
        resp->msg.payload.mig_notif_pld[0].vm_migration_status =
                VNM_VM_MIGRATION_FAILED;

        vnm_mig_notif_pld_t *req_pld = vnm_get_first_notif_pld(resp);

        //VM Details
        vnm_vm_dtls_t *req_vm_dtls = vnm_get_mig_notif_vm_dtls(req_pld);
        char ip_array[2][20] = {
        "10.32.45.12", "10.32.45.13"
        };
        char mac_array[2][20] = {
        "1012.3212.4512", "2110.3212.1245"
        };
        populate_vm_dtls(req_vm_dtls, 2, 2, (char**) ip_array,
                (char**) mac_array);

        //Local EsX Details
        vnm_esx_dtls_t *req_src_esx_dtls = vnm_get_mig_notif_pld_lcl_esx_dtls(
                req_pld);
        char kernel_ip_array[2][20] = {
        "10.32.111.12", "10.32.111.13"
        };

        populate_esx_dtls(req_src_esx_dtls, 2, VNM_ESX_SOURCE,
                (char*) "10.32.245.91", (char**) kernel_ip_array);

        //Remote ESX Details
        vnm_esx_dtls_t *req_dst_esx_dtls = vnm_get_mig_notif_pld_rmt_esx_dtls(
                req_pld);

        char dst_kernel_ip_array[2][20] = {
        "10.32.211.12", "10.32.211.13"
        };
        populate_esx_dtls(req_dst_esx_dtls, 2, VNM_ESX_DESTINATION,
                (char *)"10.32.245.45", (char**) dst_kernel_ip_array);

        //Second Payload
        vnm_mig_notif_pld_t *req_pld_next = vnm_get_next_notif_pld(req_pld);
        req_pld_next->ret_code = VNM_ERR_VMOTION_MANAGER_NO_CONNECTIVITY;

        vnm_esx_dtls_t *req_dst_esx_dtls_next =
                vnm_get_mig_notif_pld_rmt_esx_dtls(req_pld_next);
        char dst_kernel_ip_array_next[2][20] = {
        "10.2.211.12", "10.2.211.13"
        };
        populate_esx_dtls(req_dst_esx_dtls_next, 2, VNM_ESX_DESTINATION,
               (char *) "1.1.1.1", (char**) dst_kernel_ip_array_next);

        printf("Printing Response Message");
        vnm_resp_t *out = vnm_get_resp(resp);
        //Fixed Header Retrieval
        printf("Message Id %d \n", vnm_get_msg_id(out));
        printf("Response Type %d \n", vnm_get_resp_type(out));
        printf("Response Number of Payloads %d \n", vnm_get_num_of_plds(out));

        //Notification Payload Retrieval
        vnm_mig_notif_pld_t *pld = vnm_get_first_notif_pld(out);
        //payload fixed fields.
        printf("Notification Payload Cookie Task ID %d\n",
                pld->cookie.mig_task_id);
        printf("Notification Payload Return Code %d\n", pld->ret_code);
        printf("Notification Payload Migration Status %d\n",
                pld->vm_migration_status);

        //payload variable fields
        //VMDetails
        vnm_vm_dtls_t *vm_dtls = vnm_get_mig_notif_vm_dtls(pld);
        uint32_t nu_of_vm_ip = vm_dtls->num_of_ip_addrs;
        printf("Notification Payload VM Number of IP %d\n",
                vm_dtls->num_of_ip_addrs);
        uint32_t i = 0;
        for (i = 0; i < nu_of_vm_ip; i++) {
            printf("Notification Payload VM IP %s\n",
                    inet_ntoa(vm_dtls->vm_ip_addrs[i]));
        }

        uint32_t nu_of_vm_mac = vm_dtls->num_of_vm_macs;
        ether_addr_t * vm_mac_addrs = vnm_get_mig_notif_vm_mac_array(vm_dtls);
        printf("Notification Payload VM Number of Mac %d\n",
                vm_dtls->num_of_vm_macs);

        for (i = 0; i < nu_of_vm_mac; i++) {
            printf("Notification Payload VM Mac %s\n", vm_mac_addrs[i].addr);
        }

        //Local ESX Details
        vnm_esx_dtls_t *src_esx_dtls = vnm_get_mig_notif_pld_lcl_esx_dtls(pld);
        uint32_t nu_of_src_ker_ip = src_esx_dtls->num_of_kernel_ip_addrs;
        printf("Notification Payload SRC ESC Num of IP %d\n",
                src_esx_dtls->num_of_kernel_ip_addrs);
        for (i = 0; i < nu_of_src_ker_ip; i++) {
            printf("Notification Payload SRC Kernel IP %s\n",
                    inet_ntoa(src_esx_dtls->esx_kernel_ip_addrs[i]));
        }
        printf("Notification Payload SRC Kernel Role %d\n",
                src_esx_dtls->esx_role);
        printf("Notification Payload SRC Console IP %s\n",
                inet_ntoa(src_esx_dtls->esx_ip_addr));

        //Remote ESX Details
        vnm_esx_dtls_t *dst_esx_dtls = vnm_get_mig_notif_pld_rmt_esx_dtls(pld);
        uint32_t nu_of_dst_ker_ip = dst_esx_dtls->num_of_kernel_ip_addrs;
        printf("Notification Payload DST ESC Num of IP %d\n",
                dst_esx_dtls->num_of_kernel_ip_addrs);
        for (i = 0; i < nu_of_dst_ker_ip; i++) {
            printf("Notification Payload DST Kernel IP %s\n",
                    inet_ntoa(dst_esx_dtls->esx_kernel_ip_addrs[i]));
        }
        printf("Notification Payload DST Kernel Role %d\n",
                dst_esx_dtls->esx_role);
        printf("Notification Payload DST Console IP %s\n",
                inet_ntoa(dst_esx_dtls->esx_ip_addr));

        //Next Payload
        vnm_mig_notif_pld_t *pld_next = vnm_get_next_notif_pld(pld);
        printf("Notification Payload Return Code %d\n", pld_next->ret_code);

        vnm_esx_dtls_t *dst_esx_dtls_next = vnm_get_mig_notif_pld_rmt_esx_dtls(
                pld_next);
        uint32_t nu_of_dst_ker_ip_next =
                dst_esx_dtls_next->num_of_kernel_ip_addrs;
        printf("Notification Payload DST Kernel Role %d\n",
                dst_esx_dtls_next->esx_role);
        printf("Notification Payload DST Console IP %s\n",
                inet_ntoa(dst_esx_dtls_next->esx_ip_addr));
        printf("Notification Payload DST ESC Num of IP %d\n",
                dst_esx_dtls_next->num_of_kernel_ip_addrs);
        for (i = 0; i < nu_of_dst_ker_ip_next; i++) {
            printf("Notification Payload DST Kernel IP %s\n",
                    inet_ntoa(dst_esx_dtls_next->esx_kernel_ip_addrs[i]));
        }
        printf("Completed\n");

    }

    void main_power() {

        //Creating VNM_RESP of Notification Type
        vnm_resp_t *resp = allocate_memory_for_power_plds(2, 2, 2);

        resp->rsp_hdr.req_msg_id = 1000;
        resp->rsp_type = VNM_NOTIF_MIG_DTLS;

        resp->msg.num_plds = 2;

        resp->msg.payload.vm_pwd_pld[0].cookie.mig_task_id = 1203;

        resp->msg.payload.vm_pwd_pld[0].ret_code = VNM_ERR_FAILURE;
        resp->msg.payload.vm_pwd_pld[0].vnm_power_status = VMM_VM_POWERED_ON;

        vnm_vm_power_pld_t *req_pld = vnm_get_first_power_pld(resp);
        //VM Details
        vnm_vm_dtls_t *req_vm_dtls = vnm_get_mig_power_vm_dtls(req_pld);
        char ip_array[2][20] = {
        "10.32.45.12", "10.32.45.13"
        };
        char mac_array[2][20] = {
        "1012.3212.4512", "2110.3212.1245"
        };
        populate_vm_dtls(req_vm_dtls, 2, 2, (char**) ip_array,
                (char**) mac_array);

        vnm_vm_power_pld_t *pld_next = vnm_get_next_power_pld(req_pld);
        pld_next->cookie.mig_task_id = 1234;
        pld_next->ret_code = VNM_ERR_UNKNOWN;
        pld_next->vnm_power_status = VNM_VM_POWERED_OFF;
        //VM Details
        vnm_vm_dtls_t *req_vm_dtls_next = vnm_get_mig_power_vm_dtls(pld_next);
        char ip_array_next[2][20] = {
        "1.32.45.12", "1.32.45.13"
        };
        char mac_array_next[2][20] = {
        "9012.3212.4512", "9110.3212.1245"
        };
        populate_vm_dtls(req_vm_dtls_next, 2, 2, (char**) ip_array_next,
                (char**) mac_array_next);

        printf("Printing Response Message");
        vnm_resp_t *out = vnm_get_resp(resp);
        //Fixed Header Retrieval
        printf("Message Id %d \n", vnm_get_msg_id(out));
        printf("Response Type %d \n", vnm_get_resp_type(out));
        printf("Response Number of Payloads %d \n", vnm_get_num_of_plds(out));

        //Notification Payload Retrieval
        vnm_vm_power_pld_t *pld = vnm_get_first_power_pld(out);
        //payload fixed fields.
        printf("Power Payload Cookie Task ID %d\n", pld->cookie.mig_task_id);
        printf("Power Payload Return Code %d\n", pld->ret_code);
        printf("Power Payload Power Status %d\n", pld->vnm_power_status);

        //payload variable fields
        //VMDetails
        vnm_vm_dtls_t *vm_dtls = vnm_get_mig_power_vm_dtls(pld);
        uint32_t nu_of_vm_ip = vm_dtls->num_of_ip_addrs;
        printf("Notification Payload VM Number of IP %d\n",
                vm_dtls->num_of_ip_addrs);
        uint32_t i = 0;
        for (i = 0; i < nu_of_vm_ip; i++) {
            printf("Notification Payload VM IP %s\n",
                    inet_ntoa(vm_dtls->vm_ip_addrs[i]));
        }

        uint32_t nu_of_vm_mac = vm_dtls->num_of_vm_macs;
        ether_addr_t * vm_mac_addrs = vnm_get_mig_notif_vm_mac_array(vm_dtls);
        printf("Notification Payload VM Number of Mac %d\n",
                vm_dtls->num_of_vm_macs);

        for (i = 0; i < nu_of_vm_mac; i++) {
            printf("Notification Payload VM Mac %s\n", vm_mac_addrs[i].addr);
        }

        {
            vnm_vm_power_pld_t *resp_pld_next = vnm_get_next_power_pld(pld);

            //payload fixed fields.
            printf("Power Payload Cookie Task ID %d\n",
                    resp_pld_next->cookie.mig_task_id);
            printf("Power Payload Return Code %d\n", resp_pld_next->ret_code);
            printf("Power Payload Power Status %d\n",
                    resp_pld_next->vnm_power_status);
            vnm_vm_dtls_t *vm_dtls_next = vnm_get_mig_power_vm_dtls(
                    resp_pld_next);
            uint32_t nu_of_vm_ip_next = vm_dtls_next->num_of_ip_addrs;
            printf("Notification Payload VM Number of IP %d\n",
                    vm_dtls_next->num_of_ip_addrs);

            for (i = 0; i < nu_of_vm_ip_next; i++) {
                printf("Notification Payload VM IP %s\n",
                        inet_ntoa(vm_dtls_next->vm_ip_addrs[i]));
            }

            uint32_t nu_of_vm_mac_next = vm_dtls_next->num_of_vm_macs;
            ether_addr_t * vm_mac_addrs_next = vnm_get_mig_notif_vm_mac_array(
                    vm_dtls_next);
            printf("Notification Payload VM Number of Mac %d\n",
                    vm_dtls_next->num_of_vm_macs);

            for (i = 0; i < nu_of_vm_mac_next; i++) {
                printf("Notification Payload VM Mac %s\n",
                        vm_mac_addrs_next[i].addr);
            }
        }
    }*/

}
