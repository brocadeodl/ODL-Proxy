#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/L2sys/L2sysClientInterface.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "ValClientInterface/DcmClientInterface.h"
#include <assert.h>

int printL2sysClearDynamic(const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    printf(" bridge=%d\n",msg->bridge);
    printf(" Mac=%s\n",msg->Mac?msg->Mac:"NULL");
    printf(" ifname=%s\n",msg->if_name?msg->if_name:"NULL");
    printf(" vlan_id=%d\n",msg->vlan_id);
    printf(" type=%d\n",msg->type);
    printf(" cmdCode=%d\n",msg->cmdCode);
    printf(" ifindex=0x%x\n",msg->ifindex);

    return 0;
}

int printL2sysClearDynamic_address(const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    printf(" bridge=%d\n",msg->bridge);
    printf(" Mac=%s\n",msg->Mac?msg->Mac:"NULL");
    printf(" ifname=%s\n",msg->if_name?msg->if_name:"NULL");
    printf(" vlan_id=%d\n",msg->vlan_id);
    printf(" type=%d\n",msg->type);
    printf(" cmdCode=%d\n",msg->cmdCode);
    printf(" ifindex=0x%x\n",msg->ifindex);

    return 0;
}

int printL2sysClearDynamic_intf_po(const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    printf(" bridge=%d\n",msg->bridge);
    printf(" Mac=%s\n",msg->Mac?msg->Mac:"NULL");
    printf(" ifname=%s\n",msg->if_name?msg->if_name:"NULL");
    printf(" vlan_id=%d\n",msg->vlan_id);
    printf(" type=%d\n",msg->type);
    printf(" cmdCode=%d\n",msg->cmdCode);
    printf(" ifindex=0x%x\n",msg->ifindex);

    return 0;
}

int printL2sysClearDynamic_intf_te(const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    printf(" bridge=%d\n",msg->bridge);
    printf(" Mac=%s\n",msg->Mac?msg->Mac:"NULL");
    printf(" ifname=%s\n",msg->if_name?msg->if_name:"NULL");
    printf(" vlan_id=%d\n",msg->vlan_id);
    printf(" type=%d\n",msg->type);
    printf(" cmdCode=%d\n",msg->cmdCode);
    printf(" ifindex=0x%x\n",msg->ifindex);

    return 0;
}

int printL2sysClearDynamic_vlan(const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    printf(" bridge=%d\n",msg->bridge);
    printf(" Mac=%s\n",msg->Mac?msg->Mac:"NULL");
    printf(" ifname=%s\n",msg->if_name?msg->if_name:"NULL");
    printf(" vlan_id=%d\n",msg->vlan_id);
    printf(" type=%d\n",msg->type);
    printf(" cmdCode=%d\n",msg->cmdCode);
    printf(" ifindex=0x%x\n",msg->ifindex);

    return 0;
}

int l2sys_clear_mac_handler(const void *inp, void ** const out)
{
    L2sysClear_InputMsg_t *msg = (L2sysClear_InputMsg_t *)inp;
    if(msg == NULL)
        return (-1);
    switch(msg->cmdCode)
    {
        case L2SYS_CLEAR_DYNAMIC:
            return printL2sysClearDynamic(inp,out);
            break;
        case L2SYS_CLEAR_DYNAMIC_ADDRESS:
            return printL2sysClearDynamic_address(inp,out);
            break;
        case L2SYS_CLEAR_DYNAMIC_INTERFACE_PO:
            return printL2sysClearDynamic_intf_po(inp,out);
            break;
        case L2SYS_CLEAR_DYNAMIC_INTERFACE_TENGIG:
            return printL2sysClearDynamic_intf_te(inp,out);
            break;
        case L2SYS_CLEAR_DYNAMIC_VLAN:
            return printL2sysClearDynamic_vlan(inp,out);
            break;
    }

    return 0;
}

int l2sys_create_mac_handler(const void *inp, void ** const out)
{
    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;

    int mgmt_port = 9109;

    if (argc != 2) {
    	printf("usage: l2sys_app.Linux.i686 mgmt_port\n");
    	return 0;
	}

	mgmt_port += atoi(argv[1]);


    /* STEP 2 */
    setL2sysMsgHandlerForClient();

    /* STEP 3 */
    add_client_message_handler(L2SYS_CLEAR_MAC, l2sys_clear_mac_handler);

    add_client_message_handler(L2SYSCLIENTGLOBALUPDATEL2SYSGLOBALSTATICMAC, l2sys_create_mac_handler);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);


    /* STEP 4: Initialize Dcmd */
    ret = dcm_initialize_client ("l2sys");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from l2sys client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from l2sys client... \n");
    } 

    while(1)
        sleep(100);
}
