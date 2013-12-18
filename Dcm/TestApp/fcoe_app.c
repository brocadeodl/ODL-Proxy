#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"

#include "Fcoe/Common/FcoePluginTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Fcoe/FcoeClientHandler.h"

#include "DcmCStatus.h"
#include "WaveCStatus.h"

#include <assert.h>
#include "VcsFabric/Local/VcsNodeHaClientMsgCStruct.h"

int glret = WRC_WAVE_MESSAGE_SUCCESS;

#define FCOE_LOGIN_COUNT   48
#define FCOE_INTF_COUNT    48

bool g_cpActive = false;

int cpStateChangeHandler (const void *inp, void ** const out)
{
	CpState_InputMsg_t *cpStateChangeMsg = (CpState_InputMsg_t *) inp;

	uint32_t cpState = cpStateChangeMsg->cpState;
	printf ("fcoe_app::cpStateChangeHandler cpState = %d\n",cpState);
	g_cpActive = true;
	return (0);
}

int
printFcoeClientShow (const void *in_p, void ** const out_pp)
{
    fcoe_show_cmd_resp_t    *resp_p;
    fcoe_show_cmd_req_t     *req_p = (fcoe_show_cmd_req_t*)in_p;
    uint32_t                size = 0;
    uint32_t                i;

    printf("Recvd FCOE Show command code = %d\n\n", req_p->cmd_code);
    
    size = FCOE_LOGIN_COUNT * sizeof(fcoe_show_cmd_resp_t);
    
    resp_p = (fcoe_show_cmd_resp_t *)calloc(1, size);

    resp_p->cmd_code = req_p->cmd_code;
    resp_p->size = size;

    switch (resp_p->cmd_code) {
        case FCOE_SHOW_CMD_CODE_INVALID:
            break;

        case FCOE_SHOW_CMD_CODE_MAP:
            resp_p->resp.map_resp.num_entries = 1;
            sprintf((char*)resp_p->resp.map_resp.list[0].map_name, "default");
            sprintf((char*)resp_p->resp.map_resp.list[0].cee_map_name, "default");
            sprintf((char*)resp_p->resp.map_resp.list[0].fab_map_name[0], "default");
            break;

        case FCOE_SHOW_CMD_CODE_FAB_MAP:
            resp_p->resp.fab_map_resp.num_entries = 1;
            sprintf((char*)resp_p->resp.fab_map_resp.list[0].fab_map_name, "default");
            resp_p->resp.fab_map_resp.list[0].vlan_id = 1002;
            resp_p->resp.fab_map_resp.list[0].vfid = 128;
            resp_p->resp.fab_map_resp.list[0].fcmap = 0x0EFC00;
            resp_p->resp.fab_map_resp.list[0].priority = 3;
            resp_p->resp.fab_map_resp.list[0].fka_intvl = 8000;
            resp_p->resp.fab_map_resp.list[0].timeout = 0;
            break;


        case FCOE_SHOW_CMD_CODE_FIP:
            resp_p->resp.fip_resp.fka_intvl = 8000;
            resp_p->resp.fip_resp.timeout = 0;
            break;

        case FCOE_SHOW_CMD_CODE_LOGIN:
        case FCOE_SHOW_CMD_CODE_LOGIN_VLAN:
        case FCOE_SHOW_CMD_CODE_LOGIN_VFID:
        case FCOE_SHOW_CMD_CODE_LOGIN_INTF:
            resp_p->resp.login_resp.num_entries = FCOE_LOGIN_COUNT;
            for (i = 0; i < FCOE_LOGIN_COUNT; i++) {
                // uint32_t j;
                resp_p->resp.login_resp.list[i].fc_uport = i+24;
                /*
                for (j = 0; j < 8; j++) {
                    resp_p->resp.login_resp.list[i].dev_wwn[j] = i+1;
                }

                for (j = 0; j<6; j++) {
                    resp_p->resp.login_resp.list[i].dev_mac[j] = i+1;
                }

                for (j = 0; j<6; j++) {
                    resp_p->resp.login_resp.list[i].ssn_mac[j] = i+1;
                }
                */
                sprintf((char*)resp_p->resp.login_resp.list[i].dev_wwn, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                        i+1, i+1, i+1, i+1, i+1, i+1, i+1, i+1);
                sprintf((char*)resp_p->resp.login_resp.list[i].dev_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                        i+1, i+1, i+1, i+1, i+1, i+1);
                sprintf((char*)resp_p->resp.login_resp.list[i].ssn_mac, "%02x:%02x:%02x:%02x:%02x:%02x",
                        i+1, i+1, i+1, i+1, i+1, i+1);
                
            }
            break;

        case FCOE_SHOW_CMD_CODE_INTF:
        case FCOE_SHOW_CMD_CODE_INTF_BRIEF:
            resp_p->resp.intf_resp.num_entries = FCOE_INTF_COUNT;
            for (i = 0; i < FCOE_INTF_COUNT; i++) {
                resp_p->resp.intf_resp.list[i].fc_uport = i+FCOE_INTF_COUNT;
                resp_p->resp.intf_resp.list[i].if_index = i;
                snprintf((char *)resp_p->resp.intf_resp.list[i].logical_port_name,
                         FCOE_INTF_NAME_LEN_MAX, "1/1/%d", i+1);
                snprintf((char *)resp_p->resp.intf_resp.list[i].te_intf_name,
                         FCOE_PHY_INTF_NAME_LEN_MAX, "1/0/%d", i+1);
                snprintf((char *)resp_p->resp.intf_resp.list[i].cfg_mode,
                         FCOE_INTF_MODE_STR_LEN_MAX, "VF");
                snprintf((char *)resp_p->resp.intf_resp.list[i].curr_mode,
                         FCOE_INTF_MODE_STR_LEN_MAX, "VF");
            }
                
            break;

        case FCOE_SHOW_CMD_CODE_PHY_INTF_FCOE:
            break;

        default:
            printf("Invalid Code \n");
            break;
    }

    fflush(stdout);

    *out_pp = resp_p;

    return glret;
}

int
handleFcoeClearConfig (const void *in_p, void ** const out_pp)
{
    fcoe_clear_cmd_req_t    *req_p;

    req_p = (fcoe_clear_cmd_req_t *)in_p;

    printf("Recieved Fcoe Clear Command code=%d\n", req_p->cmd_code);

    switch (req_p->cmd_code) {
        case FCOE_CLEAR_LOGIN_CMD_CODE_FCOE_INTF:
        case FCOE_CLEAR_LOGIN_CMD_CODE_TE_INTF:
            printf("Rcvd clear for Uport=%u INTF=0x%llx\n", req_p->fc_uport_num, (long long unsigned int) req_p->if_index);
            break;

        case FCOE_CLEAR_LOGIN_CMD_CODE_VLAN:
            printf("Rcvd clear for VLAN=%d\n", req_p->vlan);
            break;

        case FCOE_CLEAR_LOGIN_CMD_CODE_VFID:
            printf("Rcvd clear for VFID=%d\n", req_p->vfid);
            break;

        case FCOE_CLEAR_LOGIN_CMD_CODE_DEVICE:
            printf("Rcvd clear for WWN=%llx\n", (long long unsigned int) req_p->wwn);
            break;

        case FCOE_CLEAR_COUNTERS_CMD_CODE_ALL:
            printf("Rcvd clear counters ALL\n");
            break;

        case FCOE_CLEAR_COUNTERS_CMD_CODE_UPORT:
            printf("Rcvd clear counters  for Uport=%u\n", req_p->fc_uport_num);
            break;
            
        default:
            printf("Invalid CMD Code\n");
            break;
    }

    fflush(stdout);

    return glret;
}

int
handleFcoeETIntfConfig (const void *in_p, void ** const out_pp)
{
    fcoe_intf_config_msg_t  *req_p;
    fcoe_intf_config_msg_resp_t *resp_p;
    uint32_t size;
    uint32_t i;

    req_p = (fcoe_intf_config_msg_t *)in_p;

    size = sizeof(fcoe_intf_config_msg_resp_t) + 
           (FCOE_INTF_COUNT - 1) * sizeof(fcoe_intf_confg_if_list_entry_t);
    resp_p = (fcoe_intf_config_msg_resp_t *) calloc(1, size);

    resp_p->cmd_code = req_p->cmd_code;
    resp_p->size = size;

    switch (resp_p->cmd_code) {
        case FCOE_ET_INTF_OPER_GET_IF_LIST:
            resp_p->num_entries = FCOE_INTF_COUNT;
            for (i = 0; i < FCOE_INTF_COUNT; i++) {
                resp_p->list[i].if_index = i;
                resp_p->list[i].port_num = i;
                resp_p->list[i].fc_port_num = i;
            }
            break;

        case FCOE_ET_INTF_OPER_CREATE:
        case FCOE_ET_INTF_OPER_REMOVE:
            printf("Create/Remove of ET ports from user not supported\n");
            break;

        case FCOE_ET_INTF_OPER_UPD_ISL_STATUS:
            printf("RCVD Update ISL status for IF=%s Status=%d\n",
                   req_p->list[0].if_name, req_p->list[0].status);
            break;

        case FCOE_ET_INTF_OPER_UPD_TRUNK_STATUS:
            printf("RCVD Update ISL status for IF=%s Trunk-Status=%d\n",
                   req_p->list[0].if_name, req_p->list[0].trunk_status);
            break;

        default:
            printf("Unknown ET INTF cmd code(%d) recieved\n",
                   resp_p->cmd_code);
            break;
    }

    fflush(stdout);

    *out_pp = resp_p;

    return glret;
}

int
handleFcoeIntfConfig (const void *in_p, void ** const out_pp)
{
    fcoe_intf_config_msg_t  *req_p;
    fcoe_intf_config_msg_resp_t *resp_p;
    uint32_t size;
    uint32_t i;

    req_p = (fcoe_intf_config_msg_t *)in_p;

    size = sizeof(fcoe_intf_config_msg_resp_t) + 
           (FCOE_INTF_COUNT - 1) * sizeof(fcoe_intf_confg_if_list_entry_t);
    resp_p = (fcoe_intf_config_msg_resp_t *) calloc(1, size);

    resp_p->cmd_code = req_p->cmd_code;
    resp_p->size = size;

    switch (resp_p->cmd_code) {
        case FCOE_INTF_OPER_GET_IF_LIST:
            resp_p->num_entries = FCOE_INTF_COUNT;
            for (i = 0; i < FCOE_INTF_COUNT; i++) {
                resp_p->list[i].if_index = i;
                resp_p->list[i].port_num = i;
                resp_p->list[i].fc_port_num = i;
            }
            break;

        case FCOE_INTF_OPER_CREATE:
        case FCOE_INTF_OPER_REMOVE:
            printf("Create/Remove of VF ports from user not supported\n");
            break;

        case FCOE_INTF_OPER_UPD_STATUS:
            printf("RCVD Update status for IF=%s Status=%d\n",
                   req_p->list[0].if_name, req_p->list[0].status);
            break;

        case FCOE_INTF_OPER_UPD_BIND:
            printf("RCVD Update BIND status for IF=%s Type=%d Val=%s\n",
                   req_p->list[0].if_name, req_p->list[0].bind_type,
                   req_p->list[0].bind_name);
            break;
                  

        default:
            printf("Unknown VF INTF cmd code(%d) recieved\n",
                   resp_p->cmd_code);
            break;
    }

    fflush(stdout);

    *out_pp = resp_p;

    return glret;
}

int
handleFcoeFipConfig (const void *in_p, void ** const out_pp)
{
    fcoe_fip_msg_t  *msg_p = (fcoe_fip_msg_t *)in_p;
    
    printf("Recieved FIP config command\n");
    printf("Cmd_code = 0x%x, FKA=%d, Timeout=%d\n", msg_p->oper_bmp,
           msg_p->fka_intvl, msg_p->timeout);

    fflush(stdout);

    return glret;
}

int handleFcoeFabMapConfig (const void *in_p, void ** const out_pp)
{
    fcoe_fabric_map_msg_t *msg_p = (fcoe_fabric_map_msg_t *)in_p;
    fcoe_fabric_map_msg_resp_t *resp_p = NULL;
    
    printf("Recieved FabMap config command\n");
    printf("Cmd = 0x%x, Name=%s, Vlan:%d, Vfid:%d, Fcmap:0x%x, Priority:%d"
           " FKA=%d, Timeout=%d\n", msg_p->oper_bmp,
           msg_p->fab_map_name,
           msg_p->vlan_id,
           msg_p->vfid,
           msg_p->fcmap,
           msg_p->priority,
           msg_p->fka_intvl,
           msg_p->timeout);

    resp_p = (fcoe_fabric_map_msg_resp_t *)calloc(1, sizeof(fcoe_fabric_map_msg_resp_t));
    resp_p->cmd_code = msg_p->oper_bmp;
    resp_p->size = sizeof(fcoe_fabric_map_msg_resp_t);
    resp_p->num_entries = 1;

    sprintf((char *)resp_p->fcf_mac_addr_list[0], "12:34:56:78:ab");

    fflush(stdout);

    *out_pp = resp_p;

    return glret;
}

int
handleFcoeDebugMessage (const void *in_p, void ** const out_pp)
{
    fcoe_debug_msg_t  *msg_p = (fcoe_debug_msg_t *)in_p;

    if (msg_p->oper_bmp == FCOE_DEBUG_CMD_CODE_ENABLE_LOGFILE) {
        printf("Rcvd LOGFILE Enable request\n");
    } else if (msg_p->oper_bmp == FCOE_DEBUG_CMD_CODE_DISABLE_LOGFILE) {
        printf("Rcvd LOGFILE Disable request\n");
    }

    fflush(stdout);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int handleFcoeMapConfig (const void *in_p, void ** const out_pp)
{
    return 0;
}

int
handleFcoeEthIntfConfig (const void *in_p, void ** const out_pp)
{
    fcoe_eth_intf_config_msg_t  *msg_p = NULL;
    uint32_t i = 0;
    uint32_t ifindex = 0;
    uint32_t po_id = 0;

    msg_p = (fcoe_eth_intf_config_msg_t *)in_p;

    printf("CmdCode:%d Ifindex:0x%llx IfName=%s\n",
           msg_p->cmd_code, (long long unsigned int) msg_p->if_index, msg_p->if_name);
    
    switch (msg_p->cmd_code) {
        case FCOE_ETH_INTF_OPER_FCOEPORT_LAG_ADD:
        case FCOE_ETH_INTF_OPER_FCOEPORT_LAG_DEL:
            sscanf((const char*)msg_p->if_name, "%u", &po_id);
            ifindex = 10 << 26 | po_id;
            printf("LAG-Ifindex:0x%x\n", ifindex);
            
        case FCOE_ETH_INTF_OPER_FCOEPORT_ADD:
        case FCOE_ETH_INTF_OPER_FCOEPORT_DEL:
            printf("Recieved FCOE %s Command\n",
                   msg_p->cmd_code == FCOE_ETH_INTF_OPER_FCOEPORT_ADD ?
                   "Enable" : "Disable");
            printf("Map-Name:[%s] Cee-Map-Name:[%s]\n",
                   msg_p->map_name, msg_p->cee_map_name);
            printf("Num-Vlans:%d\n", msg_p->num_vlans);
            printf("Vlans = [");
            for (i = 0; i < msg_p->num_vlans; i++) {
                printf("%d ", msg_p->fcoe_vlan_list[i]);
            }
            printf(" ]\n");
            printf("Num-Mbrs = %d\n", msg_p->num_entries);
            printf("Mbrs = [");
            for (i = 0; i < msg_p->num_entries; i++) {
                printf("0x%llx ", (long long unsigned int) msg_p->mbr_if_index_list[i]);
            }
            printf(" ]\n");
            break;

        case FCOE_ETH_INTF_OPER_LAG_ADD_MBR:
        case FCOE_ETH_INTF_OPER_LAG_DEL_MBR:
            printf("Recieved FCOE %s Command\n",
                   msg_p->cmd_code == FCOE_ETH_INTF_OPER_LAG_ADD_MBR ?
                   "Add-Mbr" : "Del-Mbr");
            printf("Num-Mbrs = %d\n", msg_p->num_entries);
            printf("Mbrs = [");
            for (i = 0; i < msg_p->num_entries; i++) {
                printf("0x%llx ", (long long unsigned int) msg_p->mbr_if_index_list[i]);
            }
            printf(" ]\n");
            break;

        default:
            printf("Invalid CMD Code:%d\n", msg_p->cmd_code);
            break;
    }

    fflush(stdout);

    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int
handleFcoePluginControl (const void *in_p, void ** const out_pp)
{
    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int main(int argc, char *argv[])
{
    int ret = 0;
    char if_name[32];
    char fcoe_if_name[32];
    int i = 0;
    int mgmt_port = 9109;
	int totalSleep = 0;

	if (argc != 2) {
		printf("usage: fcoe_app.Linux.i686 mgmt_port\n");
		return 0;
	}
	mgmt_port += atoi(argv[1]);

    setFcoeConfigHandlerForClient();

    
    /* STEP 3 */
    add_client_message_handler(FCOE_CLIENT_SHOW, printFcoeClientShow);
    add_client_message_handler(FCOE_CLIENT_INTF_CONFIG, handleFcoeIntfConfig);
    add_client_message_handler(FCOE_CLIENT_CLEAR, handleFcoeClearConfig);
    add_client_message_handler(FCOE_CLIENT_FIP_CONFIG, handleFcoeFipConfig);
    add_client_message_handler(FCOE_CLIENT_FABRIC_MAP_CONFIG, handleFcoeFabMapConfig);
    add_client_message_handler(FCOE_CLIENT_MAP_CONFIG, handleFcoeMapConfig);
    add_client_message_handler(FCOE_CLIENT_DEBUG, handleFcoeDebugMessage);
    add_client_message_handler(FCOE_CLIENT_ET_INTF_CONFIG, handleFcoeETIntfConfig);
    add_client_message_handler(FCOE_CLIENT_ETH_INTF_CONFIG, handleFcoeEthIntfConfig);
    add_client_message_handler(FCOE_CLIENT_PLUGIN_CONTROL, handleFcoePluginControl);

	/* Ha client message handling */
    add_client_message_handler(FCOE_CP_STATE_CHANGE_MSG, cpStateChangeHandler);

    WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client (FCOE_DCM_CLIENT_NAME);
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from FCOE client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from FCOE client... \n");
    } 

	while (! g_cpActive)
	{
		sleep (5);
		totalSleep += 5;
	}

	if (totalSleep < 30)
	{
		sleep (30 - totalSleep);
	}
	
    for (i = 0; i < FCOE_INTF_COUNT; i++) {
        sprintf(if_name, "1/0/%d", i);
        dcm_create_fcoe_et_interface(if_name, i, i);
        printf("Created ET interface %s\n", if_name);
        
        sprintf(fcoe_if_name, "1/1/%d", i);
        dcm_create_fcoe_interface(fcoe_if_name, 0, 
                                  FCOE_INTF_DEFAULT_BINDING,
                                  FCOE_INTERFACE_BIND_VAL_TYPE_NONE,
                                  i+FCOE_INTF_COUNT, 1,
                                  i, "Test Description");

        printf("Created FCOE interface %s\n", fcoe_if_name);
    }


    while(1)
        sleep(100);
}
