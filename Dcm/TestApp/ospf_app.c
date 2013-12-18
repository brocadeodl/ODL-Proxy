#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/OSPF/OSPFConfigHandler.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "ValClientInterface/DcmClientInterface.h"

#include <assert.h>

#include "WaveCStatus.h"
#include "DcmCStatus.h"

#define HAL_IF_CAP_EDGE     0x01
#define HAL_IF_CAP_ISL      0x02

#define SHUTDOWN 0
#define NO_SHUTDOWN 1
int printRouterOSPFConfigMsg(const void *inp, void ** const out);
int printOSPFIntfConfigMsg(const void *inp, void ** const out);
int printOSPFPermitDenyRedistributeConfigMsg(const void *inp, void ** const out);
int errorEnable = 0;

int printRouterOSPFConfigMsg(const void *inp, void ** const out)
{
    routerOSPFConfigMsg_t *msg = (routerOSPFConfigMsg_t *)inp;

    printf("OSPF Router Config msg opCode\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    printf("OSPF Config msg opCode :%d\n", msg->opCode);
    printf("OSPF Config msg cmdCode :%d\n", msg->cmdCode);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printOSPFPermitDenyRedistributeConfigMsg(const void *inp, void ** const out)
{
    OSPFPermitDenyRedistributeConfigMsg_t *msg = (OSPFPermitDenyRedistributeConfigMsg_t *)inp;

    printf("OSPF PermitDenyRedistribute Config msg opCode\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    printf("OSPF Config msg opCode :%d\n", msg->opCode);
    printf("OSPF Config msg cmdCode :%d\n", msg->cmdCode);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printOSPFIntfConfigMsg(const void *inp, void ** const out)
{
    interfaceOSPFConfigMsg_t *msg = (interfaceOSPFConfigMsg_t *)inp;

    printf("OSPF Intf Config msg\n");
    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    printf("OSPF Intf Config msg opCode :%d\n", msg->opCode);
    printf("OSPF Intf Config msg cmdCode :%d\n", msg->cmdCode);

    return WRC_WAVE_MESSAGE_SUCCESS;
}
int printOSPFAreaConfigMsg(const void *inp, void ** const out)
{
    ospf_area_config_in_msg_t *msg = (ospf_area_config_in_msg_t *)inp;

    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printOSPFAreaRangeConfigMsg(const void *inp, void ** const out)
{
    ospf_area_config_in_msg_t *msg = (ospf_area_config_in_msg_t *)inp;

    if (msg == NULL)
    	return WRC_WAVE_MESSAGE_SUCCESS;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printShowOSPFMsg(const void *inp, void ** const out)
{
	//    routerOSPFConfigMsg_t *msg = (routerOSPFConfigMsg_t *)inp;

    printf("OSPF SHOW msg opCode\n");
	return WRC_WAVE_MESSAGE_SUCCESS;

//    printf("OSPF Config msg opCode :%d\n", msg->opCode);
//    printf("OSPF Config msg cmdCode :%d\n", msg->cmdCode);

}

int main(int argc, char **argv)
{
    int ret = 0;

	int mgmt_port = 9109;

	mgmt_port += atoi(argv[1]);

    setOSPFRouterConfigMsgHandlerForClient();

    add_client_message_handler(OSPFROUTERCONFIG, printRouterOSPFConfigMsg);
    add_client_message_handler(OSPFAREA, printOSPFAreaConfigMsg);
    add_client_message_handler(OSPFAREARANGE, printOSPFAreaRangeConfigMsg);
    add_client_message_handler(OSPFINTERFACECONFIG, printOSPFIntfConfigMsg);
    add_client_message_handler(OSPFPERMITDENYCONFIG, printOSPFPermitDenyRedistributeConfigMsg);
	add_client_message_handler(OSPFSHOW, printShowOSPFMsg);
	add_client_message_handler(OSPFCLEAR, printShowOSPFMsg);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);
    
    /* Initialize Dcmd */
    ret = dcm_initialize_client ("ospf");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from ospf client... \n");
        assert(ret==0);
    }
    else
    {
       /* printf("Passed :: Dcm-Client Initialization passed from ospf client... \n");*/
    } 
    
    while(1)
        sleep(1000000);
}
