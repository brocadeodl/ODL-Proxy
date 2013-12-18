#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/VRRP/VRRPClientInterface.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "ValClientInterface/DcmClientInterface.h"

#include <assert.h>

#include "WaveCStatus.h"
#include "DcmCStatus.h"

#define HAL_IF_CAP_EDGE     0x01
#define HAL_IF_CAP_ISL      0x02

#define SHUTDOWN 0
#define NO_SHUTDOWN 1
int printVRRPIntfConfigMsg(const void *inp, void ** const out);
int errorEnable = 0;

int printVRRPIntfConfigMsg(const void *inp, void ** const out)
{
    vrrp_local_config_msg_t *msg = (vrrp_local_config_msg_t *)inp;

    printf("VRRP Intf Config msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    printf("VRRP Intf Config msg vrid:%d\n", msg->vrid);
    printf("VRRP Intf Config msg priority :%d\n", msg->prio);
    printf("VRRP Intf Config msg choice :%d\n", msg->choice);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printVRRPGlobalConfigMsg(const void *inp, void ** const out)
{
    vrrp_global_config_msg_t *msg = (vrrp_global_config_msg_t *)inp;
    printf("VRRP Global Config msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printVRRPShowMsg(const void *inp, void ** const out)
{
    vrrp_show_input_msg_t *msg = (vrrp_show_input_msg_t *)inp;
    printf("VRRP Show msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printVRRPDebugMsg(const void *inp, void ** const out)
{
    vrrp_debug_input_msg_t *msg = (vrrp_debug_input_msg_t *)inp;

    printf("VRRP Debug msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printVRRPClearMsg(const void *inp, void ** const out)
{
    vrrp_clear_stats_input_msg_t *msg = (vrrp_clear_stats_input_msg_t *)inp;

    printf("VRRP Clear msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int main(int argc, char **argv)
{
    int ret = 0;

	int mgmt_port = 9109;

	mgmt_port += atoi(argv[1]);

    setVRRPMsgHandlerForClient();
    add_client_message_handler(VRRP_CLEAR_STATS_MSG, printVRRPClearMsg);
    add_client_message_handler(VRRP_SHOW_MSG, printVRRPShowMsg);
    add_client_message_handler(VRRP_DEBUG_MSG, printVRRPDebugMsg);
    add_client_message_handler(VRRPGLOBALUPDATEVRRPGLOBALCONFIG, printVRRPGlobalConfigMsg);
    add_client_message_handler(VRRP_INTF_CONFIG_CLIENT_MSG, printVRRPIntfConfigMsg);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);
    
    /* Initialize Dcmd */
    ret = dcm_initialize_client ("vrrp");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from vrrp client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from vrrp client... \n");
    } 
    
    while(1)
        sleep(1000000);
}
