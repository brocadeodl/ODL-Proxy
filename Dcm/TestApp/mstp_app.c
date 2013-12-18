#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpMangConfigHandler.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "DcmCStatus.h"


#define PRINTF( ... ) \
    do { printf(__VA_ARGS__); fflush(stdout); } while(0)


int check_error_flag (const char *op) {
    char fname[128];
    snprintf(fname, sizeof fname, "xstp_err_%s", op);

    return access(fname, F_OK) != 0 ? 0 : WRC_MSTP_SYSTEM_ERROR;
}

/* 
 * Bridge configuration message handler 
 */
int mstp_bridge_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
	mstp_msg_br_config *config = (mstp_msg_br_config *) inp;
        printf( "        m_mode = %d\n", config->mode );
        printf( "        m_cmdCode = %d\n", config->cmd );
        printf( "        m_priority = %d\n", config->priority );
        printf( "        m_forwardDelay = %d\n", config->fwd_delay );
        printf( "        m_helloTime = %d\n", config->hello_time );
        printf( "        m_maxAge = %d\n", config->max_age );
        printf( "        m_des = %s\n", config->desc );
        printf( "        m_errDisableInterval = %d\n", config->err_timer_interval );
        printf( "        m_txHoldCount = %d\n", config->hold_count );

    if ((ret = check_error_flag("bridge_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}


/* 
 * Bridge Vlan configuration message handler 
 */
int mstp_vlan_bridge_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
	mstp_msg_br_vlan_config *config = (mstp_msg_br_vlan_config *) inp;
        printf( "        m_cmdCode = %d\n", config->cmd );
        printf( "        m_priority = %d\n", config->priority );
        printf( "        m_forwardDelay = %d\n", config->fwd_delay );
        printf( "        m_helloTime = %d\n", config->hello_time );
        printf( "        m_maxAge = %d\n", config->max_age );
        printf( "        m_agingTime = %d\n", config->aging_time );

    if ((ret = check_error_flag("bridge_vlan_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}

/* 
 * Bridge instance configuration message handler 
 */
int mstp_bridge_inst_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
	mstp_msg_br_inst_config *config = (mstp_msg_br_inst_config *) inp;

	printf("m_cmdCode:       %d \n", config->cmd);
	printf("m_instanceId:    %d \n", config->inst);
	printf("m_priority:      %d \n", config->priority);
	printf("m_vlanNum:       %d \n", config->vlan_num);

	unsigned int i = 0;
	for (i = 0; i< config->vlan_num; i++) 
	{
		printf("m_vid:        %d \n", config->vid[i]);
	}

    if ((ret = check_error_flag("bridge_inst_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}

/* 
 * Bridge port configuration message handler 
 */
int mstp_port_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
	mstp_msg_port_config *config = (mstp_msg_port_config *) inp;

	printf(" m_cmdCode = %d\n", config->cmd );
	printf(" m_name = %s\n", config->name);
	//printf(" m_if_index= %d\n", config->if_index);
	printf(" m_priority = %d\n", config->priority );
	printf(" m_helloTime = %d\n", config->hello_time );
	printf(" m_cost = %d\n", config->cost );
	printf(" m_instanceId = %d\n", config->inst );

    if ((ret = check_error_flag("port_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}

/* 
 * Port vlan configuration message handler 
 */
int mstp_port_vlan_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
    if ((ret = check_error_flag("port_vlan_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}

/* 
 * Port instance configuration message handler 
 */
int mstp_port_inst_config_handler (const void *inp, void ** const out)
{
    int ret = 0;
	printf("%s \n", __FUNCTION__);
        mstp_msg_port_config *config = (mstp_msg_port_config *) inp;

        printf(" m_cmdCode = %d\n", config->cmd );
        printf(" m_name = %s\n", config->name);
        //printf(" m_if_index= %d\n", config->if_index);
        printf(" m_priority = %d\n", config->priority );
        printf(" m_helloTime = %d\n", config->hello_time );
        printf(" m_cost = %d\n", config->cost );
        printf(" m_instanceId = %d\n", config->inst );

    if ((ret = check_error_flag("port_inst_cfg")) != 0)
        printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
	return ret;
}


int mstp_clear_handler (const void *inp, void ** const out)
{
    mstp_msg_clear_t *msg = (mstp_msg_clear_t *)inp;
    printf(" cmd = %d ifname=%s\n", msg->cmd, msg->name );
    return 0;
}


int mstp_vlan_config (const void *inp, void ** const out)
{
    int ret = 0;
    mstp_msg_vlan_config *msg = (mstp_msg_vlan_config *)inp;
    printf(" cmd = %d vlan=%d\n", msg->cmd, msg->vid );
    if ((ret = check_error_flag("vlan_cfg")) != 0)
         printf(" %s : ***** Returing error %d\n", __FUNCTION__, ret);
    fflush(stdout);
    return ret;
}

int mstp_show_span_br_handler(int req_type, int id, void ** const outp) {
    mstp_show_msg_t *p;
    mstp_show_msti_output_msg_t *mstip;
    mstp_show_if_output_msg_t *ifp;
    int num_records;

    // Assume PVST; 5 vlans (1-5) and 3 ports for each instance...
    // TODO have mechanism to dynamically change the output data
    if (req_type == REQ_TYPE_GETNEXT) {
        if (id < 0 || id > 4) return 0;
        ++id;
    } else {
        if (id < 1 || id > 5) return 0;
    }

    num_records = 1 + 3; // 1 INST + 3 IF
    *outp = p = (mstp_show_msg_t *) calloc(num_records, sizeof(mstp_show_msg_t));
    p->num_records = num_records;
    p->cmd = MSTP_SHOW_SPT_BRIEF;
    p->record_type = DCM_MSTP_SHOW_RECORD_TYPE_MSTI;
    p->more_rec_flag = (unsigned char) (id != 5);

    mstip = &p->records.inst;
    mstip->inst = id;
    mstip->br_type = NSM_BRIDGE_TYPE_PVST;
    mstip->br_mstp_enabled = 1;

    for (int i = 1; i <= 3; ++i) {
        ifp = & (++p)->records.port;
        p->record_type = DCM_MSTP_SHOW_RECORD_TYPE_IF;
        p->ifindex = 73628 + i;
        snprintf((char*)ifp->if_name, MSTP_DCM_IF_NAME_LEN, "Te 0/%d", i);
        snprintf((char*)ifp->link_type, sizeof ifp->link_type, "P2P");
    }

    return 0;
}

int mstp_show_handler(const void *inp, void ** const outp) {
    if (inp == NULL) {
        PRINTF("*** MSTPSHOW: null input..\n");
        return 0;
    }
    mstp_show_input_msg_t *in = (mstp_show_input_msg_t *) inp;
    PRINTF("MSTPSHOW: cmd=%u, type=%u, inst=%u, if_name=%s, last_index=%u\n", 
            in->cmd, in->type, in->inst, in->if_name, in->last_index);

    if (in->cmd == MSTP_SHOW_SPT_BRIEF) {
        PRINTF(" >>> show span brief");
        if (in->inst) {
            return mstp_show_span_br_handler(REQ_TYPE_GET, in->inst, outp);
        } else {
            return mstp_show_span_br_handler(REQ_TYPE_GETNEXT, in->last_index, outp);
        }
    } else {
        PRINTF(" >>> No stub yet for this command code!\n");
    }
    return 0;
}

int mstp_port_id_set_handler (const void *inp, void ** const out)
{
    unsigned int i, count = 0;
    mstp_msg_port_id_info *info;
    mstp_msg_port_id_config *config = (mstp_msg_port_id_config *) inp;

    if (config != NULL) {
        count = config->count;
        info  = config->pid_info;
    }

    printf("%s: received %u port id configs\n", __FUNCTION__, count); fflush(stdout);

    for (i = 0; i < count; ++i) {
        printf("%s: [%u] if_type=%u, if_name=%s, port_id=%u\n", __FUNCTION__, i, info->if_type, info->if_name, info->port_id); fflush(stdout);
        ++info;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;
	int port = 9109;

	if (argc != 2)
	{
		printf("usage: mstp_app.Linux.i686 mgmt_port\n");
		return 0; 
	}
	port += atoi(argv[1]);

    /* Set DCM client hander  */
    setMstpMangConfigHandlerForClient();

    /* Set MSTP cmd callback function */
    add_client_message_handler(MSTPBRIDGECONFIG, mstp_bridge_config_handler);
    add_client_message_handler(MSTPVLANBRIDGECONFIG, mstp_vlan_bridge_config_handler);
    add_client_message_handler(MSTPBRIDGEINSTCONFIG, mstp_bridge_inst_config_handler);
    add_client_message_handler(MSTPPORTCONFIG, mstp_port_config_handler);
    add_client_message_handler(MSTPPORTVLANCONFIG, mstp_port_vlan_config_handler);
    add_client_message_handler(MSTPPORTINSTCONFIG, mstp_port_inst_config_handler);
    add_client_message_handler(MSTPCLEAR, mstp_clear_handler);
    add_client_message_handler(MSTPVLANCONFIG, mstp_vlan_config);
    add_client_message_handler(MSTP_SHOW, mstp_show_handler);
    add_client_message_handler(MSTP_PORT_ID_CONFIG, mstp_port_id_set_handler);


	    
	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client("mstp");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from mstp client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from mstp client... \n");
    } 

    while(1)
        sleep(1000000);

	return ret;
}
