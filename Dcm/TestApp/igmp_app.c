#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"
#include <assert.h>
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "ClientInterface/Igmps/IgmpsClientInterface.h"
#include "ClientInterface/Igmp/IgmpMessageDef.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "DcmCStatus.h"
#include "WaveCStatus.h"

int igmpIntfCfgHandler (const void *inp, void ** const out)
{
    igmp_intf_cfg_msg_t *msg = (igmp_intf_cfg_msg_t *)inp;

	printf("%s\n", __FUNCTION__);
	printf("lmqi=%d\n", msg->lmqi);
	printf("qmrt=%d\n", msg->qmrt);
	printf("immediate_leave=%d\n", msg->immediate_leave);
	printf("query_interval=%d\n", msg->query_interval);
	printf("if_name=%s\n", msg->if_name);
	printf("if_index=%d\n", msg->if_index);
	printf("opcode=%d\n", msg->opcode);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpSgIntfCfgHandler (const void *inp, void ** const out)
{
    igmp_sg_intf_cfg_msg_t *msg = (igmp_sg_intf_cfg_msg_t *)inp;

	printf("%s\n", __FUNCTION__);
	printf("group address %s\n", msg->group);
	printf("if_name=%s\n", msg->if_name);
	printf("if_type=%d\n", msg->if_type);
	printf("negation=%d\n", msg->negation);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpsGlobalCfgHandler (const void *inp, void ** const out)
{
	igmps_global_config_msg_t *msg = (igmps_global_config_msg_t *)inp;

	printf("%s\n", __FUNCTION__);
	printf("enable=%d\n", msg->enable);
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpsVlanCfgHandler (const void *inp, void ** const out)
{
    igmps_vlan_config_msg_t *msg = (igmps_vlan_config_msg_t *)inp;

    printf("%s\n", __FUNCTION__);
    printf("enable=%d\n", msg->enable);
    printf("qenable=%d\n", msg->qenable);
    printf("fast_leave=%d\n", msg->fast_leave);
    printf("qmrt=%d\n", msg->qmrt);
    printf("lmqi=%d\n", msg->lmqi);
    printf("query_interval=%d\n", msg->query_interval);
    printf("mrouter_timeout=%d\n", msg->mrouter_timeout);
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpsSgVlanCfgHandler (const void *inp, void ** const out)
{
	igmps_sg_vlan_config_msg_t *msg = (igmps_sg_vlan_config_msg_t *)inp;
	
	printf("%s\n", __FUNCTION__);
	printf("group address %s\n", msg->group);
    printf("if_name=%s\n", msg->if_name);
    printf("if_type=%s\n", msg->if_type);
    printf("negation=%c\n", msg->negation);
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpsMrtrVlanCfgHandler (const void *inp, void ** const out)
{
    igmps_mrtr_vlan_config_msg_t *msg = (igmps_mrtr_vlan_config_msg_t *)inp;

    printf("%s\n", __FUNCTION__);
    printf("if_name=%s\n", msg->if_name);
    printf("if_type=%s\n", msg->if_type);
    printf("negation=%c\n", msg->negation);
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int igmpsShowHandler (const void *inp, void ** const out)
{
    igmps_show_input_msg_t *msg = (igmps_show_input_msg_t *)inp;

    printf("%s\n", __FUNCTION__);

    switch (msg->cmdCode) {
        case IGMPS_SHOW_GROUPS_ALL_CLI:
            printf("case IGMPS_SHOW_GROUPS_ALL_CLI\n"); 
            break;

        case IGMPS_SHOW_GROUPS_ALL_DETAIL_CLI:
            printf("case IGMPS_SHOW_GROUPS_ALL_DETAIL_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_ADDR_CLI:
            printf("case IGMPS_SHOW_GROUPS_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_ADDR_DETAIL_CLI:
            printf("case IGMPS_SHOW_GROUPS_DETAIL_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_INTERFACE_CLI:
            printf("case IGMPS_SHOW_GROUPS_INTERFACE_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_INTERFACE_DETAIL_CLI:
            printf("case IGMPS_SHOW_GROUPS_INTERFACE_DETAIL_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_INTERFACE_ADDR_CLI:
            printf("case IGMPS_SHOW_GROUPS_INTERFACE_ADDR_CLI\n");
            break;

        case IGMPS_SHOW_GROUPS_INTERFACE_ADDR_DETAIL_CLI:
            printf("case IGMPS_SHOW_GROUPS_INTERFACE_ADDR_DETAIL_CLI\n");
            break;

        case IGMPS_SHOW_INTERFACE_CLI:
            printf("case IGMPS_SHOW_INTERFACE_CLI\n");
            break;

        case IGMPS_SHOW_INTERFACE_VLAN_CLI:
            printf("case IGMPS_SHOW_INTERFACE_VLAN_CLI\n");
            break;

        case IGMP_SHOW_INTERFACE_VE_CLI:
            printf("case IGMP_SHOW_INTERFACE_VE_CLI\n");
            break;

        case IGMP_SHOW_INTERFACE_PHY_CLI:
            printf("case IGMP_SHOW_INTERFACE_PHY_CLI\n");
            break;

        case IGMP_SHOW_INTERFACE_PO_CLI:
            printf("case IGMP_SHOW_INTERFACE_PO_CLI\n");
            break;

        case IGMPS_SHOW_STATISTICS_CLI:
            printf("case IGMPS_SHOW_STATISTICS_CLI\n");
            break;

        case IGMP_SHOW_STATISTICS_INTERFACE_PHY_CLI:
            printf("case IGMP_SHOW_STATISTICS_INTERFACE_PHY_CLI\n");
            break;

        case IGMP_SHOW_STATISTICS_INTERFACE_PO_CLI:
            printf("case IGMP_SHOW_STATISTICS_INTERFACE_PO_CLI\n");
            break;

        default:
            printf("Error. Unrecognized Command");
            break;
    }

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int main(int argc, char **argv)
{
    int ret = 0;
	int port = 9109;

	if (argc != 2) {
		printf("usage: igmp_app.Linux.i686 <port>\n");
		return 0; 
	}
	port += atoi(argv[1]);

    /* Set DCM client hander */
    /* Call the IGMPs function since we're using a single instantiator for both 
     * IGMP & IGMP snooping */
    setIgmpsMsgHandlerForClient();

    add_client_message_handler(IGMPCLIENTPHYINTFLOCALCONFIG, igmpIntfCfgHandler);
    add_client_message_handler(IGMPCLIENTPOSVIINTFLOCALCONFIG, igmpIntfCfgHandler);
    add_client_message_handler(IGMPCLIENTSGINTFLOCALCONFIG, igmpSgIntfCfgHandler);
    add_client_message_handler(IGMPSCLIENTUPDATEIGMPSCONFIG, igmpsGlobalCfgHandler);
    add_client_message_handler(IGMPSCLIENTUPDATEVLANCONFIG, igmpsVlanCfgHandler);
    add_client_message_handler(IGMPSCLIENTUPDATESGVLANCONFIG, igmpsSgVlanCfgHandler);
    add_client_message_handler(IGMPSCLIENTUPDATEMRTRVLANCONFIG, igmpsMrtrVlanCfgHandler);
    add_client_message_handler(IGMPSCLIENTSHOW, igmpsShowHandler);
	
    WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client("IGMP");
    if (ret != 0 ) {
        printf("Failed :: Dcm-client Initialization failed from IGMP client... \n");
        assert(ret==0);
    } else {
        printf("Passed :: Dcm-Client Initialization passed from IGMP client... \n");
    } 

    while(1)
        sleep(1000000);

	return ret;
}

