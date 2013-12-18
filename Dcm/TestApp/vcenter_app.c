#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/vCenter/vCenterClientHandler.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"

#include <netinet/in.h>
#include "DcmCStatus.h"
#include "WaveCStatus.h"
#include <netinet/ether.h>
#include <assert.h>

int glret = WRC_WAVE_MESSAGE_SUCCESS;

void print_power_pld(vnm_resp_t *out) {
    /*  printf("Printing Response Message");
     printf("Message Id %d \n", vnm_get_msg_id(out));
     printf("Response Type %d \n", vnm_get_resp_type(out));
     printf("Response Number of Payloads %d \n", vnm_get_num_of_plds(out));
     vnm_vm_power_pld_t *pld = vnm_get_first_power_pld(out);
     printf("Power Payload Cookie Task ID %d\n", pld->cookie.mig_task_id);
     printf("Power Payload Return Code %d\n", pld->ret_code);
     printf("Power Payload Power Status %d\n", pld->vnm_power_status);
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
     ether_addr_t *vm_mac_addrs = vnm_get_mig_notif_vm_mac_array(vm_dtls);
     printf("Notification Payload VM Number of Mac %d\n",
     vm_dtls->num_of_vm_macs);
     for (i = 0; i < nu_of_vm_mac; i++) {
     printf("Notification Payload VM Mac %s\n", vm_mac_addrs[i].addr);
     }
     {
     vnm_vm_power_pld_t *resp_pld_next = vnm_get_next_power_pld(pld);
     printf("Power Payload Cookie Task ID %d\n",
     resp_pld_next->cookie.mig_task_id);
     printf("Power Payload Return Code %d\n", resp_pld_next->ret_code);
     printf("Power Payload Power Status %d\n",
     resp_pld_next->vnm_power_status);
     vnm_vm_dtls_t *vm_dtls_next = vnm_get_mig_power_vm_dtls(resp_pld_next);
     uint32_t nu_of_vm_ip_next = vm_dtls_next->num_of_ip_addrs;
     printf("Notification Payload VM Number of IP %d\n",
     vm_dtls_next->num_of_ip_addrs);
     for (i = 0; i < nu_of_vm_ip_next; i++) {
     printf("Notification Payload VM IP %s\n",
     inet_ntoa(vm_dtls_next->vm_ip_addrs[i]));
     }
     uint32_t nu_of_vm_mac_next = vm_dtls_next->num_of_vm_macs;
     ether_addr_t *vm_mac_addrs_next = vnm_get_mig_notif_vm_mac_array(
     vm_dtls_next);
     printf("Notification Payload VM Number of Mac %d\n",
     vm_dtls_next->num_of_vm_macs);
     for (i = 0; i < nu_of_vm_mac_next; i++) {
     printf("Notification Payload VM Mac %s\n",
     vm_mac_addrs_next[i].addr);
     }
     }*/
}

int handlevCenterEvents(const void *in_p, void ** const out_pp) {
    vnm_resp_t *out = (vnm_resp_t *) in_p;

    printf(
            "**************************Printing Response Message***********************\n");

    //Fixed Header Retrieval

    printf("Message Id %d \n", vnm_get_msg_id(out));
    printf("Response Type %d \n", vnm_get_resp_type(out));
    printf("Response Number of Payloads %d \n", vnm_get_num_of_plds(out));

    //Notification Payload Retrieval
    vnm_mig_notif_pld_t *pld = vnm_get_first_notif_pld(out);
    //payload fixed fields.
    printf("\nvCenter IP %s\n", convert_ip_to_string(pld->cookie.vcenter_ip));
    printf("Notification Payload Cookie Event ID %d\n",
            pld->cookie.mig_task_id);
    printf("Notification Payload Cookie VM ID %s\n", pld->cookie.vm_id);
    printf("Notification Payload Return Code %d\n", pld->ret_code);

    printf("Notification Payload Migration Status %d\n",
            pld->vm_migration_status);

    //payload variable fields
    //VMDetails
    vnm_vm_dtls_t *vm_dtls = vnm_get_mig_notif_vm_dtls(pld);

    uint32_t nu_of_vm_mac = vm_dtls->num_of_vm_macs;
    struct ether_addr * vm_mac_addrs = vnm_get_mig_notif_vm_mac_array(vm_dtls);
    printf("Notification Payload VM Number of Mac %d\n",
            vm_dtls->num_of_vm_macs);
    uint32_t i = 0;
    for (i = 0; i < nu_of_vm_mac; i++) {

        printf("Notification Payload VM Mac %s\n",
                ether_ntoa(&vm_mac_addrs[i]));
    }
    uint32_t num_of_mac_ip_maps = vm_dtls->num_of_mac_ip_maps;
    printf("Notification Payload VM Number of IP MAC Mappings %d\n",
            vm_dtls->num_of_mac_ip_maps);

    for (i = 0; i < num_of_mac_ip_maps; i++) {
        printf("Notification Payload VM IP  MAC %s %s\n",
                convert_ip_to_string(vm_dtls->vnm_mac_ip_maps[i].vm_ip_addr),
                ether_ntoa(&vm_dtls->vnm_mac_ip_maps[i].vm_mac));

    }

    //Local ESX Details
    vnm_esx_dtls_t *src_esx_dtls = vnm_get_mig_notif_pld_lcl_esx_dtls(pld);
    uint32_t nu_of_src_ker_ip = src_esx_dtls->num_of_kernel_ip_addrs;
    printf("Notification Payload SRC ESC Num of IP %d\n",
            src_esx_dtls->num_of_kernel_ip_addrs);
    for (i = 0; i < nu_of_src_ker_ip; i++) {
        printf("Notification Payload SRC Kernel IP %s\n",
                convert_ip_to_string(src_esx_dtls->esx_kernel_ip_addrs[i].ip));
        printf("Kernel Port is of Type %d %s\n",src_esx_dtls->esx_kernel_ip_addrs[i].nic_type,
                vnm_is_nicType_Management(src_esx_dtls->esx_kernel_ip_addrs[i].nic_type)?"Management":"not Management");

    }
    printf("Notification Payload SRC Kernel Role %d\n", src_esx_dtls->esx_role);
    printf("Notification Payload SRC Console IP %s\n",
            convert_ip_to_string(src_esx_dtls->esx_ip_addr));

    //Remote ESX Details
    vnm_esx_dtls_t *dst_esx_dtls = vnm_get_mig_notif_pld_rmt_esx_dtls(pld);
    uint32_t nu_of_dst_ker_ip = dst_esx_dtls->num_of_kernel_ip_addrs;
    printf("Notification Payload DST ESC Num of IP %d\n",
            dst_esx_dtls->num_of_kernel_ip_addrs);
    for (i = 0; i < nu_of_dst_ker_ip; i++) {
        printf("Notification Payload DST Kernel IP %s\n",
                convert_ip_to_string(dst_esx_dtls->esx_kernel_ip_addrs[i].ip));
        printf("Kernel Port is of Type %d %s\n",dst_esx_dtls->esx_kernel_ip_addrs[i].nic_type,
                       vnm_is_nicType_Management(dst_esx_dtls->esx_kernel_ip_addrs[i].nic_type)?"Management":"not Management");
    }
    printf("Notification Payload DST Kernel Role %d\n", dst_esx_dtls->esx_role);
    printf("Notification Payload DST Console IP %s\n",
            convert_ip_to_string(dst_esx_dtls->esx_ip_addr));

    //Next Payload
    /*    vnm_mig_notif_pld_t *pld_next = vnm_get_next_notif_pld(pld);
     printf("Notification Payload Return Code %d\n", pld_next->ret_code);

     vnm_esx_dtls_t *dst_esx_dtls_next = vnm_get_mig_notif_pld_rmt_esx_array(
     pld_next);
     uint32_t nu_of_dst_ker_ip_next = dst_esx_dtls_next->num_of_kernel_ip_addrs;
     printf("Notification Payload DST Kernel Role %d\n",
     dst_esx_dtls_next->esx_role);
     printf("Notification Payload DST Console IP %s\n",
     inet_ntoa(dst_esx_dtls_next->esx_ip_addr));
     printf("Notification Payload DST ESC Num of IP %d\n",
     dst_esx_dtls_next->num_of_kernel_ip_addrs);
     for (i = 0; i < nu_of_dst_ker_ip_next; i++) {
     printf("Notification Payload DST Kernel IP %s\n",
     inet_ntoa(dst_esx_dtls_next->esx_kernel_ip_addrs[i]));
     }*/

    printf("***************Completed************************\n");

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int main(int argc, char *argv[]) {
    int ret = 0;
    int mgmt_port = 9109;

    if (argc < 2) {
        printf("usage: vcenter_app.Linux.i686 mgmt_port vm_mor listen\n");
        return 0;
    }
    mgmt_port += atoi(argv[1]);

    setvCenterMsgHandlerForClient();

    add_client_message_handler(VCENTEREVENTS, handlevCenterEvents);

    WaveNs
    ::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client_for_dual_daemons("vCenterDcmClient");
    printf("Power Status\n");
    vnm_req_t *req = (vnm_req_t *) malloc(sizeof(vnm_req_t));
    req->num_plds = 1;
    req->req_type = VNM_REQ_VM_POWER_STATUS_USING_COOKIE;
    req->payload.cookie[0].mig_task_id = 1265;
    struct vnm_ip_addr_t vm_ip;
    convert_string_to_ip(&vm_ip, "10.32.45.67");
    req->payload.cookie[0].vcenter_ip = vm_ip;
    strcpy(req->payload.cookie[0].vm_id, argv[2]);

    uint32_t message_id;
    vnm_resp_t *vnm_resp;
    vnm_request(*req, &message_id, &vnm_resp);
    if (vnm_resp) {

        printf(
                "\nvCenter IP %s\n",
                convert_ip_to_string(
                        vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vcenter_ip));

        //Success
        if (vnm_resp->msg.payload.vm_pwd_pld[0].ret_code == VNM_ERR_SUCCESS) {
            printf("VM-MOR %s\n",
                    (vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vm_id));

            if (vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status
                    == VMM_VM_POWERED_ON) {
                printf("Retrieving Power Status Completed %d %s\n",
                        vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,
                        "VM Powered ON");
            }
            else if (vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status
                    == VNM_VM_POWERED_OFF) {
                printf("Retrieving Power Status Completed %d %s\n",
                        vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,
                        "VM Powered OFF");
            }
            else if (vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status
                    == VNM_VM_SUSPENDED) {
                printf("Retrieving Power Status Completed %d %s\n",
                        vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,
                        "VM Suspended");
            }
        }
        else {
            printf("Retrieving Power Status Failed : Return Code = %d\n",
                    vnm_resp->msg.payload.vm_pwd_pld[0].ret_code);
        }

    }

    if (ret != 0) {
        printf(
                "Failed :: Dcm-client Initialization failed from vCenter client... \n");
        assert(ret==0);
    }
    else {
        printf(
                "Passed :: Dcm-Client Initialization passed from vCenter client... \n");
    }

    if (argc == 4)
        while (1)
            sleep(100);
}
