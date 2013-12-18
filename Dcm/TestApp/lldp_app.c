#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/Lldp/LldpDcmClientHandler.h"

#include "DcmCStatus.h"
#include "WaveCStatus.h"

#include <assert.h>

int glret = WRC_WAVE_MESSAGE_SUCCESS;

int handleLldpFcoePriority  (const void *in_p, void ** const out_pp)
{
    lldp_fcoe_priority_conf_t   *msg_p = (lldp_fcoe_priority_conf_t *)in_p;
    
    printf ("RCVD FCOE priority Message Priority=%d\n",
            msg_p->priority_bitmap);
            
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printscalars(const void *inp, void ** const out);
int printprofile(const void *inp, void ** const out);
int printinterface(const void *inp, void ** const out);

int
printscalars(const void *inp, void ** const out)
{
	lldp_mgmt_conf_req_t *req_p = (lldp_mgmt_conf_req_t *)inp;

	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DESC))
		printf("description:: %s\n",req_p->entry.global_conf.description);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_HELLO))
		printf("hello:: %d\n",req_p->entry.global_conf.hello);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_MODE))
		printf("mode:: %d\n",req_p->entry.global_conf.mode);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_MULTIPLIER))
		printf("multiplier:: %d\n",req_p->entry.global_conf.multiplier);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DCBXFCOEAPPTLV))
		printf("dcbx_fcoeapp_tlv:: %d\n",req_p->entry.global_conf.dcbx_fcoeapp_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DCBXFCOELOGICALLINKTLV))
		printf("dcbx_fcoelogicallink_tlv:: %d\n",req_p->entry.global_conf.dcbx_fcoelogicallink_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DCBXTLV))
		printf("dcbx_tlv:: %d\n",req_p->entry.global_conf.dcbx_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DOT1XTLV))
		printf("dot1x_tlv:: %d\n",req_p->entry.global_conf.dot1x_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DOT3XTLV))
		printf("dot3x_tlv:: %d\n",req_p->entry.global_conf.dot3x_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_MGMTADDRTLV))
		printf("mgmtaddr_tlv:: %d\n",req_p->entry.global_conf.mgmtaddr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_PORTDESCRTLV))
		printf("portdescr_tlv:: %d\n",req_p->entry.global_conf.portdescr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_SYSCAPTLV))
		printf("syscap_tlv:: %d\n",req_p->entry.global_conf.syscap_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_SYSDESCRTLV))
		printf("sysdescr_tlv:: %d\n",req_p->entry.global_conf.sysdescr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_SYSNAMETLV))
		printf("sysname_tlv:: %d\n",req_p->entry.global_conf.sysname_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_SYSNAME))
		printf("sysname:: %s\n",req_p->entry.global_conf.sysname);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_SYSDESCR))
		printf("sysdescr:: %s\n",req_p->entry.global_conf.sysdescr);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_DISABLE))
		printf("disable:: %d\n",req_p->entry.global_conf.disable);

	return 1;
}

int
printprofile(const void *inp, void ** const out)
{
	lldp_mgmt_conf_req_t *req_p = (lldp_mgmt_conf_req_t *)inp;
		
	printf("mgmt_flags:: %d\n",req_p->mgmt_flags);
	printf("cmd_code:: %d\n",req_p->cmd_code);
	printf("msg_type:: %d\n",req_p->msg_type);

	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_CONF_PROFILE))
		printf("profile_name:: %s\n",req_p->entry.profile_conf.profile_name);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DESC))
		printf("description:: %s\n",req_p->entry.profile_conf.description);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_HELLO))
		printf("hello:: %d\n",req_p->entry.profile_conf.hello);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_MODE))
		printf("mode:: %d\n",req_p->entry.profile_conf.mode);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_MULTIPLIER))
		printf("multiplier:: %d\n",req_p->entry.profile_conf.multiplier);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOEAPPTLV))
		printf("dcbx_fcoeapp_tlv:: %d\n",req_p->entry.profile_conf.dcbx_fcoeapp_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DCBXFCOELOGICALLINKTLV))
		printf("dcbx_fcoelogicallink_tlv:: %d\n",req_p->entry.profile_conf.dcbx_fcoelogicallink_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DCBXTLV))
		printf("dcbx_tlv:: %d\n",req_p->entry.profile_conf.dcbx_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DOT1XTLV))
		printf("dot1x_tlv:: %d\n",req_p->entry.profile_conf.dot1x_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_DOT3XTLV))
		printf("dot3x_tlv:: %d\n",req_p->entry.profile_conf.dot3x_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_MGMTADDRTLV))
		printf("mgmtaddr_tlv:: %d\n",req_p->entry.profile_conf.mgmtaddr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_PORTDESCRTLV))
		printf("portdescr_tlv:: %d\n",req_p->entry.profile_conf.portdescr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_SYSCAPTLV))
		printf("syscap_tlv:: %d\n",req_p->entry.profile_conf.syscap_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_SYSDESCRTLV))
		printf("sysdescr_tlv:: %d\n",req_p->entry.profile_conf.sysdescr_tlv);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_GLOBAL_PROFILE_CONF_SYSNAMETLV))
		printf("sysname_tlv:: %d\n",req_p->entry.profile_conf.sysname_tlv);

		return 1;
}
int
printinterface(const void *inp, void ** const out)
{
	lldp_mgmt_conf_req_t *req_p = (lldp_mgmt_conf_req_t *)inp;
	
	printf("mgmt_flags:: %d\n",req_p->mgmt_flags);
	printf("cmd_code:: %d\n",req_p->cmd_code);
	printf("msg_type:: %d\n",req_p->msg_type);
	
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_INTF_CONF_DISABLE))
		printf("disable:: %d\n",req_p->entry.intf_conf.disable);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_INTF_CONF_DCBXVERSION))
		printf("dcbx version:: %d\n",req_p->entry.intf_conf.dcbx_version);
	if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_INTF_CONF_PROFILE))
		printf("profile_name:: %s\n",req_p->entry.intf_conf.profile_name);
    if (IS_CMDCODE_SET(req_p->cmd_code, LLDP_INTF_CONF_DESCRIPTION))
        printf("description :: %s\n", req_p->entry.intf_conf.description);
	
	return WRC_WAVE_MESSAGE_SUCCESS;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int mgmt_port = 9109;

	if (argc != 2) {
		printf("usage: lldp_app.Linux.i686 mgmt_port\n");
		return 0;
	}
	mgmt_port += atoi(argv[1]);

    setLldpDcmClientMsgHandler();

    add_client_message_handler(MSG_LLDP_FCOE_PRIORITY_CONF, handleLldpFcoePriority);
    add_client_message_handler(MSG_LLDP_SCALARS_GLOBAL_CONF, printscalars);
    add_client_message_handler(MSG_LLDP_PROFILE_GLOBAL_CONF, printprofile);
    add_client_message_handler(MSG_LLDP_PHY_INTF_CONF, printinterface);

    WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client (LLDP_DCM_CLIENT_NAME);
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from LLDP client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from LLDP client... \n");
    } 
    
    while(1)
        sleep(100);
}
