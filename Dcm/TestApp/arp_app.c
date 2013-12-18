#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/Nsm/NsmMangIntfHandler.h"
#include "ClientInterface/Nsm/NsmClientCreateIntIpConfigSPMessage.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/Arp/ArpClientInterface.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
arp_show_out_msg_t *g_payload_p;
int g_arp_count = 5;
int g_walk_count=0;
int arp_set(dcm_arp_rec  *rec, unsigned int count );
void* arp_log_alloc(unsigned int payload_size);
int arp_show_summary (arp_show_in_msg_t *msg, void ** const out);
int arp_show_ip(arp_show_in_msg_t *msg, void ** const out);
int arp_show_only(arp_show_in_msg_t *msg, void ** const out);
int arp_show_interface(arp_show_in_msg_t *msg, void ** const out);
int arp_show_mac(arp_show_in_msg_t *msg, void ** const out);
int arp_show(const void *inp, void ** const out);


int arp_set(dcm_arp_rec  *rec, unsigned int count ) {
    for(unsigned int i=0; i< count; i++) {   
        strncpy(rec[i].ip, "1.1.1.1", ARP_DCMP_ARP_REC_IP_STR_SIZE);
        strncpy(rec[i].mac, "0000.1111.2222",ARP_DCMP_ARP_REC_MAC_STR_SIZE);
        strncpy(rec[i].type, "test",ARP_DCMP_ARP_REC_TYPE_STR_SIZE);
        strncpy(rec[i].if_name,"te 0/1", ARP_DCMP_ARP_REC_IF_NAME_STR_SIZE);
        strncpy(rec[i].is_resolved,"resolved", ARP_DCMP_ARP_REC_IS_RESOLVED_STR_SIZE);
        strncpy(rec[i].time, "00:10:10 ",ARP_DCMP_ARP_REC_TIME_STR_SIZE);
    }
    return 0;
}

void* arp_log_alloc(unsigned int payload_size) {
    return malloc(payload_size);
}

int arp_show_summary (arp_show_in_msg_t *msg, void ** const out)
{
    unsigned int payload_size;
    arp_show_out_msg_t *payload_p;
    msg->summary = true;
    payload_size = sizeof(arp_show_out_msg_t);
    payload_p = (arp_show_out_msg_t *) arp_log_alloc(payload_size);

    if ((SHOW_ARP_DYNAMIC_CONFIG == msg->cmd && true == msg->summary)
        || SHOW_ARP_SUMMARY == msg->cmd) {
    payload_p->dynamic_arp_count = 1;
    }
    if ((SHOW_ARP_STATIC_CONFIG == msg->cmd && true == msg->summary)
        || SHOW_ARP_SUMMARY == msg->cmd) {
    payload_p->static_arp_count = 0;
    }

    payload_p->size = payload_size;
    *out = payload_p;
    return(0);
}



int arp_show_ip(arp_show_in_msg_t *msg, void ** const out)
{
    int payload_size = 0;
    unsigned int ip_addr;
    int arp_index;

    //msg->summary = PAL_FALSE;
    inet_pton(AF_INET, msg->ip, &ip_addr);
    /* allocate pay load based on total active ARP count */
    //printf("arp count %u \n",g_arp_count);
    if(g_arp_count > 0) {
        payload_size = sizeof(arp_show_out_msg_t) + (sizeof(dcm_arp_rec));
    }
    else {
        payload_size = sizeof(arp_show_out_msg_t);
    }
    //  printf("p[ayload size  %u \n",payload_size);

    g_payload_p = (arp_show_out_msg_t *) arp_log_alloc(payload_size);
    g_payload_p->size = payload_size;
    arp_index =  1;
    if(arp_index != 0){

        if ( true) {
            g_payload_p->static_arp_count = 1;
            g_payload_p->dynamic_arp_count = 0;
        }
        else {
            g_payload_p->static_arp_count = 0;
            g_payload_p->dynamic_arp_count = 1;
        }
        
    }
    else{
        g_payload_p->static_arp_count = 0;
        g_payload_p->dynamic_arp_count = 0;
    }

    /* copy the payload */
    *out = g_payload_p;
    return 0;

}

int arp_page_count = 0;
int arp_show_only(arp_show_in_msg_t *msg, void ** const out)
{
    int payload_size = 0;

    g_walk_count=0;
    /* this will have either the static or dynamic or both based on the type sent */
    unsigned int count_arps = 0;

    if ( (SHOW_ARP_DYNAMIC_CONFIG == msg->cmd ||
         SHOW_ARP_STATIC_CONFIG == msg->cmd )
         && true == msg->summary ) {
        int rc = arp_show_summary(msg,out);
        return rc;
    }

    if (SHOW_ARP_DYNAMIC_CONFIG == msg->cmd) {
        count_arps = 5;
    }
    else if (SHOW_ARP_STATIC_CONFIG == msg->cmd) {
        count_arps = 5;
    }
    else {
        count_arps = g_arp_count;
    }
    //msg->summary = PAL_FALSE;

    /* allocate pay load based on total active ARP count */
    //printf("arp count %u \n",g_arp_count);
    if(count_arps > 0) {
        payload_size = sizeof(arp_show_out_msg_t) + ((count_arps - 1) * sizeof(dcm_arp_rec));
    }
    else {
        payload_size = sizeof(arp_show_out_msg_t);
    }
    //  printf("p[ayload size  %u \n",payload_size);
   g_payload_p = (arp_show_out_msg_t *) arp_log_alloc(payload_size);
    if (SHOW_ARP_DYNAMIC_CONFIG == msg->cmd) {
        g_payload_p->dynamic_arp_count = 5;
        g_payload_p->static_arp_count = 0;
    }
    else if (SHOW_ARP_STATIC_CONFIG == msg->cmd) {
        g_payload_p->dynamic_arp_count = 0;
        g_payload_p->static_arp_count = 5;
    }
    else {
        g_payload_p->dynamic_arp_count = 5;
        g_payload_p->static_arp_count = 0;
    }
    g_payload_p->size = payload_size;
    /* Start the entire walk of ARP and store the elements into payload */
    //g_arp_table_walk(arp_walk, (void *)msg->cmd, NULL);
    //HeapSort((void *)g_payload_p->rec, sizeof(dcm_arp_rec), arp_compare_function, count_arps);
    arp_set(g_payload_p->rec, count_arps);
    if(arp_page_count == 3) {
      g_payload_p->more_elements = 0;
      arp_page_count = 0;
    }
    else {
        arp_page_count++;
        g_payload_p->more_elements =1;
    }

    g_payload_p->rec_count=5;
                
    g_walk_count=0; /* Reset the walk count */

    /* copy the payload */
    *out = g_payload_p;
    return 0;

}

int arp_show_interface(arp_show_in_msg_t *msg, void ** const out)
{
    int payload_size = 0;
    unsigned int arp_interface_count=5;
    g_walk_count=0;

    /* allocate pay load based on total ARP count on a ifindex  */
    if(arp_interface_count > 0) {
        payload_size = sizeof(arp_show_out_msg_t) + ((arp_interface_count - 1) * sizeof(dcm_arp_rec));
    }
    else {
        payload_size = sizeof(arp_show_out_msg_t);
    }

    g_payload_p = (arp_show_out_msg_t *) arp_log_alloc(payload_size);
    g_payload_p->dynamic_arp_count = arp_interface_count;
    g_payload_p->static_arp_count = 0;
    g_payload_p->size = payload_size;
    /* Start the entire walk of ARP and store the elements into payload */
    //arp_walk_by_ifindex(ifindex, arp_pack_payload_by_ifindex, NULL);
    //g_arp_table_walk(arp_walk, NULL, (void *)ifindex);
    //HeapSort((void *)g_payload_p->rec, sizeof(dcm_arp_rec), arp_compare_function, arp_interface_count);
    //g_walk_count=0;
    arp_set(g_payload_p->rec, arp_interface_count);

    /* copy the payload */
    *out = g_payload_p;
    return 0;
}

int arp_show_mac(arp_show_in_msg_t *msg, void ** const out)
{
    int payload_size = 0;
    int arp_mac_count = 0;
    g_walk_count = 0;
    int arp_static_mac_count = 3; int arp_dynamic_mac_count = 2;
    unsigned char mac_addr [18] = {'\0'};
    int ret = 0;

    if ((ret = sscanf(msg->mac, "%4hx.%4hx.%4hx",
                    (unsigned short *)&mac_addr[0],
                    (unsigned short *)&mac_addr[2],
                    (unsigned short *)&mac_addr[4])) != 3) {
        printf( "%% Error: Unable to translate mac address %s %d",mac_addr, ret);
        return (0);
    }
    
    arp_mac_count = arp_static_mac_count + arp_dynamic_mac_count;
    /* allocate pay load based on total ARP count on a mac  */
    if(arp_mac_count > 0) {
        payload_size = sizeof(arp_show_out_msg_t) + ((arp_mac_count - 1) * sizeof(dcm_arp_rec));
    }
    else {
        payload_size = sizeof(arp_show_out_msg_t);
    }

    g_payload_p = (arp_show_out_msg_t *) arp_log_alloc(payload_size);
    g_payload_p->dynamic_arp_count = arp_static_mac_count;
    g_payload_p->static_arp_count = arp_dynamic_mac_count;
    g_payload_p->size = payload_size;
    /* Start the entire walk of ARP and store the elements into payload */
    //g_arp_table_mac_walk(arp_walk, NULL, (void *)mac_addr);
    //HeapSort((void *)g_payload_p->rec, sizeof(dcm_arp_rec), arp_compare_function, arp_mac_count);
    //g_walk_count=0;
    arp_set(g_payload_p->rec, arp_mac_count);

    /* copy the payload */
    *out = g_payload_p;

    return 0;
}


int
arp_show(const void *inp, void ** const out)
{
    arp_show_in_msg_t *msg = (arp_show_in_msg_t *)inp;
    printf("vrf name = %s \n", msg->vrf_name);
    printf("cmd code = %d , if name = %s , static arps = %d, summary = %d, dynamic = %d, ip = %s , mac = %s, ip_mask = %s, skip count = %d\n", msg->cmd, 
        msg->if_name, msg->static_arps, msg->summary, msg->dynamic, msg->ip, msg->mac, msg->ip_mask, msg->skip_count);
    unsigned int rc=0;

    switch(msg->cmd) {
        case SHOW_ARP_SUMMARY:
             printf("SHOW_ARP_SUMMARY cmd code  =  %d\n", msg->cmd);
             rc = arp_show_summary(msg,out);
             return(rc);
             break;
        case SHOW_ARP_IP_CONFIG:
             printf("SHOW_ARP_IP_CONFIG cmd code  =  %d\n", msg->cmd);
             rc = arp_show_ip(msg,out);
             return(rc);
             break;
        case SHOW_ARP:
        case SHOW_ARP_STATIC_CONFIG:
        case SHOW_ARP_DYNAMIC_CONFIG: {
             rc = arp_show_only(msg,out);
             printf("SHOW_ARP & SHOW_ARP_STATIC_CONFIG & SHOW_ARP_DYNAMIC_CONFIG cmd code = %d\n", msg->cmd);
             return(rc);
             break;
        }
        case SHOW_ARP_TE_IF_CONFIG:
        case SHOW_ARP_VLAN_IF_CONFIG:
        case SHOW_ARP_PO_IF_CONFIG:
        case SHOW_ARP_GI_IF_CONFIG:
        case SHOW_ARP_FO_IF_CONFIG: {

             printf("SHOW_ARP_TE_IF_CONFIG & SHOW_ARP_VLAN_IF_CONFIG & SHOW_ARP_PO_IF_CONFIG & SHOW_ARP_GI_IF_CONFIG & SHOW_ARP_FO_IF_CONFIG cmd code = %d\n",  msg->cmd);
             return(rc);
             rc = arp_show_only(msg,out);
             return(rc);
             break;
        }
        case SHOW_ARP_MAC_CONFIG:
            rc = arp_show_mac(msg, out);
            printf("SHOW_ARP_MAC_CONFIG cmd code = %d\n",  msg->cmd);
            return (rc);
            break;
        default:
             //printf("cmd %d ",msg->cmd);
             printf("default cmd code = %d\n", msg->cmd);
             return(rc);
             break;

    }
    // printf("arp show command arp_count is %u\n",g_arp_count);
    //  arp_count = g_arp_count;
        return (0);
}


int arp_ip_config_handler (const void *inp, void ** const out)
{
    nsm_ip_global_config_msg_t  *msg = (nsm_ip_global_config_msg_t *)inp;

    printf("%s %s \n", "TestApp/arp_app.c - ", __FUNCTION__);
    printf("arp_app.c :: ip_config_handler() ... \n");

    if (msg == NULL)
    {
        printf("Error: --> msg is Null\n");
        return (-1);
    }

    printf ("%s(), if_name = %s\n", __FUNCTION__, msg->if_name);
    printf ("%s(), intf_type = %d\n", __FUNCTION__, msg->intf_type);
    printf ("%s(), opcode = %d\n", __FUNCTION__, msg->opcode);
    printf ("%s(), mtu = %d\n", __FUNCTION__, msg->ip_mtu);
    printf ("%s(), proxy-arp = %d\n", __FUNCTION__, msg->proxy_arp);
    printf ("%s(), art-aging-timeout = %d\n", __FUNCTION__, msg->arp_timeout);
    printf("\n\n");

    *out = NULL;

    return (0);
}
#if 0
int arp_show__handler (const void *inp, void ** const out)
{
    arp_show_in_msg_t  *m = (arp_show_in_msg_t*) inp;
    int n = 5;
    int i = 0;
    int size = sizeof(arp_show_out_msg_t) + sizeof(dcm_arp_rec)* (n-1);
    arp_show_out_msg_t *msg = (arp_show_out_msg_t*) malloc(sizeof(arp_show_out_msg_t) + sizeof(dcm_arp_rec)* (n-1));


}
#endif

int arp_static_config_handler (const void *inp, void ** const out)
{
    arp_static_config_msg_t *msg = (arp_static_config_msg_t*)inp;

    printf("%s %s \n", "TestApp/arp_app.c - ", __FUNCTION__);
    printf("arp_app.c :: arp_static_config_handler() ... \n");

    if (msg == NULL)
    {
        printf("Error: --> msg is Null\n");
        return (-1);
    }

    printf ("%s(), vrf_ctxt = %s\n", __FUNCTION__, msg->vrf_ctxt);
    printf ("%s(), if_name = %s\n", __FUNCTION__, msg->if_name);
    printf ("%s(), intf_id = %d\n", __FUNCTION__, msg->interface_id);
    printf ("%s(), intf_type = %d\n", __FUNCTION__, msg->interface_type);
    printf ("%s(), ip_address = %s\n", __FUNCTION__, msg->ip_address);
    printf ("%s(), mac_address = %s\n", __FUNCTION__, msg->mac_address);


    printf("\n\n");

    *out = NULL;

    return (0);
}


int main(int argc, char **argv)
{
    int ret = 0;
    int mgmt_port = 9109;

    mgmt_port += atoi(argv[1]);

    /* Set DCM client hander  */
    setARPMsgHandlerForClient();

    add_client_message_handler(MSG_IP_INT, arp_ip_config_handler);
    add_client_message_handler(ARPSTATICCONFIGCLIENT, arp_static_config_handler);
    add_client_message_handler(ARPSHOW, arp_show);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client("arp");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from ARP client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from ARP client... \n");
    } 

    while(1)
        sleep(1000000);
}
