#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimClientInterface.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "DcmCStatus.h"
#include "WaveCStatus.h"

int pim_dcm_show_pim_neighbor_handler(const void *inp, void ** const out)
{
    printf("%s \n", __FUNCTION__);
    printf(" Plugin Mes Received:\n");
#if 0
    //pim_show_out_msg *msg = (pim_show_out_msg *) malloc(sizeof(pim_show_out_msg));
	 //msg->size = sizeof(pim_show_out_msg);
	//	 msg->dr_priority = 1;
//	 strcpy(msg->port_str, "port1"); 
      //   msg->rtr_address.afi = 10;
        // msg->hold_down_time = 12;
       //  msg->nbr_timer = 13;
      //   msg->nbr_uptime = 14;
 //        strcpy(msg->vrf_name, "vrf1");
        // msg->dr_priority_present = 1;
//	*out = msg;
#endif /* if 0 */
    printf(" Return Mes Populated:\n");
	return 1;
}

/* 
 * PIM router config message handler 
 */
int pim_rtr_cfg_handler (const void *inp, void ** const out)
{
    *out = NULL;
	printf("%s \n", __FUNCTION__);
	pim_rtr_cfg_msg_t *config = (pim_rtr_cfg_msg_t *) inp;
    //printf( "        vrf_id = %s\n", config->vrf_id);
    printf( "        cmd_code = %d\n", config->cmd_code);
    printf( "        msg_type = %d\n", config->msg_type);
    printf( "        max_mcache = %d\n", config->max_mcache);
    printf( "        hello_timer = %d\n", config->hello_timer);
    printf( "        nbr_to = %d\n", config->nbr_to);
    printf( "        inact_timer = %d\n", config->inact_timer);
    printf( "        msg_intv = %d\n", config->msg_intv);
    printf( "        spt_thold = %u\n", config->spt_thold);
    if(config->msg_type == PIM_MSG_DEL && 
       config->cmd_code == PIM_CFG_SPT_THOLD_CMD) {
        config->spt_thold = 1; 
        printf( "        spt_thold = %u\n", config->spt_thold);
    }
	return WRC_WAVE_MESSAGE_SUCCESS;
}

/* 
 * PIM BSR Candidate message handler 
 */
int pim_bsr_candidate_handler (const void *inp, void ** const out)
{
    *out = NULL;
	printf("%s \n", __FUNCTION__);
	pim_bsr_candidate_msg_t *config = (pim_bsr_candidate_msg_t *) inp;
    printf( "        Msg Type = %d\n", config->msg_type);
    printf( "        Intf Type = %d\n", config->intf_type);
    printf( "        Intf Name = %s\n", config->intf_name);
    printf( "        hash_mask = %d\n", config->hash_mask);
    printf( "        priority = %d\n", config->priority);
	
	return WRC_WAVE_MESSAGE_SUCCESS;
}

/* 
 * PIM router config message handler 
 */
int pim_ip_mult_cfg_handler (const void *inp, void ** const out)
{
    *out = NULL;
	printf("%s \n", __FUNCTION__);
	pim_ip_mult_cfg_msg_t *config = (pim_ip_mult_cfg_msg_t *) inp;
    printf( "        enable = %d\n", config->enable);

	return WRC_WAVE_MESSAGE_SUCCESS;
}

/*
 * Interface related PIM config commands 
 */
int pim_phy_intf_cfg_handler (const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
	pim_phy_intf_cfg_msg_t *config = (pim_phy_intf_cfg_msg_t *) inp;
    printf( "        enable = %d\n", config->enable);
    printf( "        dr_prio = %d\n", config->dr_prio);
//    printf( "        mult_bdry = %s\n", config->mult_bdry);
    printf( "        brdr_rtr = %d\n", config->brdr_rtr);
    printf( "        ttl_thold = %d\n", config->ttl_thold);
    printf( "        Intf Type = %d\n", config->intf_type);
    printf( "        Intf Name = %s\n", config->intf_name);
	
	return WRC_WAVE_MESSAGE_SUCCESS;
}

/*
 * Static RP PIM config commands 
 */
int pim_static_rp_cfg_handler (const void *inp, void ** const out)
{
    *out = NULL;
    printf("%s \n", __FUNCTION__);
    pim_static_rp_msg_t *config = (pim_static_rp_msg_t *) inp;
    printf( "        Msg Type = %d\n", config->msg_type);
    printf( "        RP Addr = %s\n", config->rp_addr);
    printf( "        Prefix List = %s\n", config->prefix_list);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

/*
 * Candidate RP PIM config commands 
 */
int pim_cand_rp_cfg_handler (const void *inp, void ** const out)
{
    *out = NULL;
    printf("%s \n", __FUNCTION__);
    pim_cand_rp_cfg_msg_t *config = (pim_cand_rp_cfg_msg_t *) inp;
    printf( "        Msg Type = %d\n", config->msg_type);
    printf( "        Intf Type = %d\n", config->intf_type);
    printf( "        Intf Name = %s\n", config->intf_name);
    printf( "        Prefix List = %s\n", config->prefix_list);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

/*
 * Candidate RP Prefix PIM config command
 */
int pim_cand_rp_prefix_cfg_handler (const void *inp, void ** const out)
{
    *out = NULL;
    printf("%s \n", __FUNCTION__);
    pim_cand_rp_prefix_msg_t *config = (pim_cand_rp_prefix_msg_t *) inp;
    printf( "        Msg Type = %d\n", config->msg_type);
    printf( "        Prefix Action = %d\n", config->prefixAction);
    printf( "        Prefix IP = %s\n", config->prefixIP);
    printf( "        Prefix Length = %d\n", config->prefixLen);

    return WRC_WAVE_MESSAGE_SUCCESS;
}


int main(int argc, char **argv)
{
    int ret = 0;
	int port = 9109;

	if (argc != 2)
	{
		printf("usage: pim_app.Linux.i686 pim_port\n");
		return 0; 
	}
	port += atoi(argv[1]);

    /* Set DCM client hander  */
    setPimMsgHandlerForClient();

    /* Set MSTP cmd callback function */
    add_client_message_handler(PIM_CFG_RTR_PIM, pim_rtr_cfg_handler);
    add_client_message_handler(PIM_CFG_IP_MULT, pim_ip_mult_cfg_handler);
    add_client_message_handler(PIM_BSR_CANDIDATE, pim_bsr_candidate_handler);
    add_client_message_handler(PIM_INTF_CFG_PIM_SP, pim_phy_intf_cfg_handler);
    add_client_message_handler(PIM_SHOW_NEIGHBOR, pim_dcm_show_pim_neighbor_handler);
    add_client_message_handler(PIM_CFG_STATIC_RP, pim_static_rp_cfg_handler);
    add_client_message_handler(PIM_CFG_CAND_RP, pim_cand_rp_cfg_handler);
	add_client_message_handler(PIM_CFG_CAND_RP_PREFIX, pim_cand_rp_prefix_cfg_handler);
/*    add_client_message_handler(MSTPPORTVLANCONFIG, mstp_port_vlan_config_handler);
    add_client_message_handler(MSTPPORTINSTCONFIG, mstp_port_inst_config_handler);
    add_client_message_handler(MSTPCLEAR, mstp_clear_handler);
    add_client_message_handler(MSTPVLANCONFIG, mstp_vlan_config);*/
	    
	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client("pim");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from pim client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from pim client... \n");
    } 

    while(1)
        sleep(1000000);

	return ret;
}
