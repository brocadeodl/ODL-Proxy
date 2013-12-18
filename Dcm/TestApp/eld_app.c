#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/Eld/EldClientInterface.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#include <assert.h>

void show_interface_status(eldshow_interface_inputmsg_t *msg)
{
    printf("Within function show_interface_status().\r\n");
	return;
}

void show_interface_status_for_rbridge(eldshow_interface_inputmsg_t *msg,
									   void ** const out)
{
	eld_show_msg_t *payload_p = NULL;
    printf("Within function show_interface_status_for_rbridge().\r\n");
	printf("Need to show data for rbridge: %d\n", msg->rbridge_id);
	
	payload_p = (eld_show_msg_t *)malloc(sizeof(eld_show_msg_t));

	if (payload_p == NULL) {
		printf("MALLOC FAILED. \n");
		return;
	}

	payload_p->num_records = 1;
	payload_p->record_type = ELD_SHOW_RBRIDGE;
    /* compiling error with the following lines
	payload_p->data.eldshow_if[0].testvar = 45;
	payload_p->data.eldshow_if[0].rbridge_id = msg->rbridge_id;
    */

	*out = payload_p;
	return;
}

int execute_show_cmd(const void *inp, void ** const out)
{
    eldshow_interface_inputmsg_t *msg = (eldshow_interface_inputmsg_t *)inp;
    int cmdCode = 0;

    if(msg == NULL)
        return (-1);

    printf("eld_dcm_show_handler : entry\r\n");

    cmdCode = msg->cmdcode;
    if (cmdCode == ELD_SHOW_INTF_VLAN)
    {
        printf("eld_dcm_show_handler : calling show_interface_status\r\n");
        show_interface_status(msg);
    }

    if (cmdCode == ELD_SHOW_RBRIDGE)
    {
        printf("eld_dcm_show_handler : calling show_interface_status\r\n");
        show_interface_status_for_rbridge(msg, out);
		return 0;
    }

//	int size = sizeof(eld_show_msg_t) + sizeof (eldshow_interface_outputmsg_t);
	eld_show_msg_t *payload_p = (eld_show_msg_t *)malloc(sizeof(eld_show_msg_t));
//	eld_show_msg_t *payload_p = (eld_show_msg_t *)malloc(size);

	if (payload_p == NULL) {
		printf("MALLOC FAILED. \n");
		return -1;
	}

	payload_p->num_records = 1;
//	payload_p->num_records = 2;
	payload_p->record_type = ELD_SHOW_INTF_VLAN;
    /* compiling error with the following lines
	payload_p->data.info[0].testvar = 2;
	payload_p->data.info[0].rbridge_id = 32;
    */
//	payload_p->data.info[1].testvar = 41;
//	payload_p->data.info[1].rbridge_id = 98;

	*out = payload_p;
    return 0;
}

int execute_eld_po_interface_cmd(const void *inp, void ** const out)
{
    eld_mgmt_conf_req_t *req_p = (eld_mgmt_conf_req_t *)inp;

    if(req_p == NULL)
        return (-1);

    switch(req_p->entry.intf_conf.cmd_code)
    {
        case ELD_INTF_PO_CONF_ELD_ENABLE:
        printf ("execute_eld_po_interface_cmd : ELD_INTF_PO_CONF_ELD_ENABLE\r\n");
    	printf ("execute_eld_po_interface_cmd code %d PO-id %s PO-Name %s\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.po_info.poId, 
									req_p->entry.intf_conf.po_info.poName);
        break;
        case ELD_INTF_PO_CONF_ELD_DISABLE:
        printf ("execute_eld_po_interface_cmd : ELD_INTF_PO_CONF_ELD_DISABLE\r\n");
    	printf ("execute_eld_po_interface_cmd code %d PO-id %s PO-name %s\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.po_info.poId, 
									req_p->entry.intf_conf.po_info.poName);
        break;
        case ELD_INTF_PO_CONF_PRIORITY:
        printf ("execute_eld_po_interface_cmd : ELD_INTF_PO_CONF_PRIORITY\r\n");
    	printf ("execute_eld_po_interface_cmd code %d PO-id %s PO-name %s priority %d\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.po_info.poId, 
									req_p->entry.intf_conf.po_info.poName, 
									req_p->entry.intf_conf.priority);
        break;
	}

	return 0;
}	

int execute_eld_interface_cmd(const void *inp, void ** const out)
{
    eld_mgmt_conf_req_t *req_p = (eld_mgmt_conf_req_t *)inp;

    if(req_p == NULL)
        return (-1);

    switch(req_p->entry.intf_conf.cmd_code)
    {
        case ELD_INTF_CONF_ELD_ENABLE:
        printf ("execute_eld_global_cmd : ELD_INTF_CONF_ELD_ENABLE\r\n");
    	printf ("execute_eld_interface_cmd code %d if %s\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.if_name);
        break;
        case ELD_INTF_CONF_ELD_DISABLE:
        printf ("execute_eld_global_cmd : ELD_INTF_CONF_ELD_DISABLE\r\n");
    	printf ("execute_eld_interface_cmd code %d if %s\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.if_name);
        break;
        case ELD_INTF_CONF_PRIORITY:
        printf ("execute_eld_global_cmd : ELD_INTF_CONF_PRIORITY\r\n");
    	printf ("execute_eld_interface_cmd code %d if %s priority %d\r\n",
        							req_p->entry.intf_conf.cmd_code, 
									req_p->entry.intf_conf.if_name, 
									req_p->entry.intf_conf.priority);
        break;
	}

	return 0;
}	

int execute_eld_global_cmd(const void *inp, void ** const out)
{
    eld_mgmt_conf_req_t *req_p = (eld_mgmt_conf_req_t *)inp;

    if(req_p == NULL)
        return (-1);

    printf ("Entering execute_eld_global_cmd \r\n");

    switch(req_p->entry.global_conf.cmd_code)
    {
        case ELD_GLOBAL_CONF_HELLO_TIME:
        printf ("execute_eld_global_cmd : ELD_GLOBAL_CONF_HELLO_TIME\r\n");
        break;
        case ELD_GLOBAL_CONF_SHUTDOWN_TIME:
        printf ("execute_eld_global_cmd : ELD_GLOBAL_CONF_SHUTDOWN_TIME\r\n");
        break;
        case ELD_GLOBAL_CONF_VCS_RX_LIMIT:
        printf ("execute_eld_global_cmd : ELD_GLOBAL_CONF_VCS_RX_LIMIT\r\n");
        break;
    } 

    return 0;
}

int execute_clear_cmd(const void *inp, void ** const out)
{
    int cmdCode = 0;
    eld_clear_msg_t *req_p = (eld_clear_msg_t *)inp;

    if(req_p == NULL)
        return (-1);

    printf ("Entering execute_eld_clear_cmd \r\n");

    cmdCode = req_p->cmdcode;
    if (cmdCode == ELD_GLOBAL_CLEAR)
    {
        printf("eld_dcm_clear_handler : calling ....\r\n");
    }
	return 0;
}

int execute_eld_debug_cmd(const void *inp, void ** const out)
{
    int cmdCode = 0;
    eld_debug_msg_t *req_p = (eld_debug_msg_t *)inp;

    if(req_p == NULL)
        return (-1);

    printf ("Entering execute_eld_debug_cmd \r\n");

    cmdCode = req_p->cmdcode;
    if (cmdCode == ELD_INTERFACE_DEBUG)
    {
        printf("eld_dcm_debug_handler : calling ....\r\n");
    }
	return 0;
}


int main(int argc, char **argv)
{
    int ret = 0;

    int mgmt_port = 9109;

    if (argc != 2) {
    	printf("usage: eld.Linux.i686 mgmt_port\n");
    	return 0;
	}

	mgmt_port += atoi(argv[1]);


    /* STEP 2 */
    setEldMsgHandlerForClient();

    /* STEP 3 */
    add_client_message_handler(MSG_ELD_DEBUG, execute_eld_debug_cmd);
    add_client_message_handler(MSG_ELD_CLEAR, execute_clear_cmd);
    add_client_message_handler(MSG_ELD_SHOW_INTERFACE_VLAN, execute_show_cmd);
    add_client_message_handler(MSG_ELD_SCALARS_GLOBAL_CONF, execute_eld_global_cmd);
	add_client_message_handler(MSG_ELD_INTF_CONF, execute_eld_interface_cmd);
    add_client_message_handler(MSG_ELD_INTF_PO_CONF, execute_eld_po_interface_cmd);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);


    /* STEP 4: Initialize Dcmd */
    ret = dcm_initialize_client ("eld");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from eld client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from eld client... \n");
    } 

    while(1)
        sleep(100);
}
