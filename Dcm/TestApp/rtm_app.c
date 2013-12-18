#include <stdio.h>
#include <stdarg.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Rtm/RtmClientInterface.h"
#include "WyserEaGateway/GenericClientMessage.h"

static int rtmRdHandler(const void *inp, void ** const out) 
{
    printf("RTM: %s\n", __FUNCTION__);
    return 0;
}



static int rtm_config_handler (const void *inp, void ** const out)
{
    rtm_config_msg_t *msg = (rtm_config_msg_t*) inp;
	printf("RTM: %s\n", __FUNCTION__);
    if(!msg)  {
        printf("inp NULL \n");
        return 0;
    }
    printf("opcode = %u \n", msg->opCode);
    printf("vrfName = %s \n", msg->vrfName);
    printf("routerID = %s \n", msg->routerID);
    printf("vrfRdRt = %s \n", msg->vrfRdRt);
    printf("vrfRt = %s \n", msg->vrfRt);
    printf("vrfRt = %d \n", msg->vrfRtType);
    printf("afIpv4Mode = %s \n", msg->afIpv4Mode ? "TRUE":"FALSE");
    printf("maxRoutes = %u \n", msg->maxRoutes);
	return 0;
}

static int rtm_static_route_handler (const void *inp, void ** const out)
{
    rtm_static_route_msg_t *msg = (rtm_static_route_msg_t*) inp;
	printf("RTM: %s\n", __FUNCTION__);
    if(!msg)  {
        printf("inp NULL \n");
        return 0;
    }
    printf("opcode = %u \n", msg->opCode);
    printf("vrfCtxt = %s \n\
            nextHopVrf = %s \n\
            nhIp = %s \n\
            dest = %s \n", msg->vrfCtxt, msg->nextHopVrf, msg->nhIp, msg->dest);
	return 0;
}

static int rtm_show_cmd_handler(const void *inp, void ** const out)
{
    printf("RTM: %s\n", __FUNCTION__);
    rtm_show_in_msg_t* msg = (rtm_show_in_msg_t*) inp;
    for (unsigned int i = 0 ; i< msg->argc; ++i) {
        printf("argv[%d] = %s\n", i, msg->argv[i]);
    }
  	return 0;
}

static int rtm_debug_cmd_handler(const void *inp, void ** const out)
{
	printf("RTM: %s\n", __FUNCTION__);
	return 0;
}

static int rtm_nodebug_cmd_handler(const void *inp, void ** const out)
{
	printf("RTM: %s\n", __FUNCTION__);
	return 0;
}

static int rtm_clear_cmd_handler(const void *inp, void ** const out)
{
//  unsigned int i; 
  generic_wyser_msg_t *msg = (generic_wyser_msg_t*) inp;
	printf("RTM: %s\n", __FUNCTION__);
  if(NULL == msg)
      return 0;
#if 0
  for(i=0; i < msg->argc; i++) {
      printf("%s ", msg->argv[i]);
  }
  printf("\n");
#endif
	return 0;
}

/*
 * Init dcm client object and add callback function 
 */
int main(int argc, char **argv)
{
	int ret = 0;
    int mgmt_port = 9109;

    mgmt_port += atoi(argv[1]);

	/* Set DCM client hander  */
	setRtmMsgHandlerForClient();

	/* Set rtm cmd callback function */
    add_client_message_handler(MSG_VRF_INT, rtmRdHandler);
	add_client_message_handler(RTMRTMCONFIG, rtm_config_handler);
	add_client_message_handler(RTMSTATICROUTE, rtm_static_route_handler);
	add_client_message_handler(RTMSHOW, rtm_show_cmd_handler);
	add_client_message_handler(RTMCLEAR, rtm_clear_cmd_handler);
	add_client_message_handler(RTMDEBUG, rtm_debug_cmd_handler);
	add_client_message_handler(RTMNODEBUG, rtm_nodebug_cmd_handler);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);
	/* Initialize Dcmd */
	ret = dcm_initialize_client (RTM_DCM_CLIENT_NAME);

    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from RTM client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from RTM client... \n");
    } 

    while(1)
        sleep(1000000);

	return ret;

}
