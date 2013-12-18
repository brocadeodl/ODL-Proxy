#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Ssm/SsmClientInterface.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "DcmCStatus.h"
#include "WaveCStatus.h"
#include "ssmAcl_dcmd_public.h"
#include "ssmBum_dcmd_public.h"
#include "Ssm/Local/SsmTypes.h"

#if 0
int printIntAssocMsg(void *inp, int opcode);
int printIntAssocMsg(void *inp, int opcode) {
	intAssocMsg_t *msg = (intAssocMsg_t *) inp;
	printf("Interface Name: %s\n", msg->ifname);
	printf("Policy Name: %s\n", msg->policyname);
	printf("Opcode:%d\n", msg->opcode);
	return (0);
}
#endif


#define ACL_MAC_FMT_STR "%02x%02x.%02x%02x.%02x%02x"
#define ACL_MAC_VAL(x)  (x)[0], (x)[1], (x)[2],(x)[3],(x)[4],(x)[5]

#define ACL_IPV4_FMT_STR  "%d.%d.%d.%d"
#define ACL_IPV4_VAL_UINT32(x)   (((uint8_t *)&x)[0]), (((uint8_t *)&x)[1]), (((uint8_t *)&x)[2]), (((uint8_t *)&x)[3])


int ssm_dcm_bum_control_handler(const void *inp, void ** const out);
int stormcontrol_dcm_clear_handler(const void *inp, void ** const out);
int stormcontrol_dcm_show_handler(const void *inp, void ** const out);
int printAclRule(const void *inp, void ** const out);
int printAclPolicy(const void *inp, void ** const out);
int ssm_dcm_show_handler(const void *inp, void ** const out);
int ssm_dcm_clear_handler(const void *inp, void ** const out);
int printAclBulkPolicy(const void *inp, void ** const out);
int printAclStats(const void *inp, void ** const out);
int printIPv4AclRule(const void *inp, void ** const out);
int printIPv4AclPolicy(const void *inp, void ** const out);
int printAclResequenc(const void *inp, void ** const out);

int qosProfileHandler(const void *inp, void ** const out);
int secProfileHandler(const void *inp, void ** const out);
int fcoePHandler( const void *inp, void ** const out );

int printAclStats(const void *inp, void ** const out)
{
	L2aclIntfStatsMsg_t *pmsg = (L2aclIntfStatsMsg_t *)inp;
	 L2aclIntfStatsMsg_t *resp = NULL;

	int num = 2;
	int i;		
	


	if (pmsg == NULL)
		return (-1);

	printf("***********************************************************\n");
	printf("Input ifname: %s\n", pmsg->ifname);
	printf("Input opcode: %d\n", pmsg->opcode);


	resp = (L2aclIntfStatsMsg_t *)calloc(1, sizeof(L2aclIntfStatsMsg_t) + (num - 1) *  sizeof (ruleStats_t));

	if (resp != NULL) {
		strcpy(resp->policyname, "test");
		resp->policytype = MAC_ACL_POLICY_STANDARD;
		strcpy(resp->ifname, "te 0/0");
		resp->iftype = PHY_INTF_TYPE;
		resp->numrecords = 2;
		for (i = 0; i < num; i++) {
			resp->stats[i].ruleid = i + 10;
			resp->stats[i].countframes = i + 100;
		}

		*out = resp;
		return (65538);
	} else {
		*out = NULL;
		return (-1);
	}



}
		
int printAclBulkPolicy(const void *inp, void ** const out)
{
	L2aclPolicyBulkMsg_t *pmsg = (L2aclPolicyBulkMsg_t *)inp;
	L2aclRuleMsg_t *msg;
	unsigned long long i;


	if (pmsg == NULL)
		return (-1);

	 	printf("***********************************************************\n");
		printf("Input POLICYNAME: %s\n", pmsg->policyname);
		printf("Input POLICYTYPE: %d\n", pmsg->policytype);
		printf("Input NUMRULES: %llu\n", pmsg->numrules);
		printf("Input POLOPCODE: %d\n", pmsg->opcode);
		for (i = 0; i < pmsg->numrules; i++) {
			printf("--------------------------------------------------\n");
			msg = (L2aclRuleMsg_t *)&(pmsg->rules[i]);
			printf("Input PolicyName: %s\n", msg->policyname);
			printf("Input policytype: %d\n", msg->policytype);
			printf("Input opcode: %d\n", msg->opcode);
			printf("Input Ruleid: %llu\n", msg->ruleid);
			printf("Input action: %d\n", msg->action);
			printf("Input srcmac: %s srcMask: %d\n", msg->srcmac, msg->srcmask);
			printf("Input dstmac: %s dstMask: %d\n", msg->dstmac, msg->dstmask);
			printf("Input apptype: %s iscount:%d\n", msg->apptype, msg->rulecounter);
			printf("--------------------------------------------------\n");
		}
	 printf("***********************************************************\n");

	*out = NULL;

	return (65538);
			
	
}

int printIPv4AclPolicy(const void *inp, void ** const out)
{
	ssmAcl_dcmd_acl_t *msg = (ssmAcl_dcmd_acl_t *) inp;

	if (msg == NULL) {
		return (-1);
	}

	printf("Opcode is %d \n", msg->opcode );
	printf("PolicyType is %d \n", msg->type);
	printf("PolicyName is %s \n", msg->acl_name);
	printf("Intf Name is %s \n", msg->if_name);
	printf("Direction is %d \n", msg->direction);

	/* This is mandatory. If you have structure assing the valid memory
	 * otherwise assign null
	 */
	*out = NULL;

	return (65538);
}	
int printAclResequence(const void *inp, void ** const out)
{
	ssmAcl_dcmd_resequence_t *msg = (ssmAcl_dcmd_resequence_t *) inp;
    if (msg == NULL) {
        return (-1);
    }

    printf("Opcode is %d \n", msg->opcode);
    printf("PolicyType is %d \n", msg->type);
    printf("PolicyName is %s \n",msg->acl_name);
    
    printf("BaseSeqid is %d \n", ((uint32_t)(msg->seqId)));
	printf("IncrementalSeqid is %d \n",((uint32_t)(msg->incSeqId)));

    /* This is mandatory. If you have structure assing the valid memory
     * otherwise assign null
     */
    *out = NULL;

    return (65538);	
}

int printIPv4AclRule(const void *inp, void ** const out)
{
	unsigned long long *ruleid = NULL;
	static unsigned long long incr = 0;

	ssmAcl_dcmd_l3_rule_t *msg = (ssmAcl_dcmd_l3_rule_t *) inp;
	if (msg == NULL) {
		printf ("Ruleid: %d doesn't exists\n",((uint32_t)((msg->rule).generic).seq_num));
		return (-1);
	}

	printf("Value of opcode is %d \n", msg->opcode );
	printf ("Sequence number %d %d \n",msg->user_entered_sequence_number,
		 ((uint32_t)((msg->rule).generic).seq_num));
	printf ("Source IP %x "ACL_IPV4_FMT_STR"/"ACL_IPV4_FMT_STR"\n",
	  msg->rule.src_ip.valid,
	  ACL_IPV4_VAL_UINT32(msg->rule.src_ip.addr),
	  ACL_IPV4_VAL_UINT32(msg->rule.src_ip.mask));
	printf ("Dest IP %x "ACL_IPV4_FMT_STR"/"ACL_IPV4_FMT_STR"\n",
	  msg->rule.dst_ip.valid,
	  ACL_IPV4_VAL_UINT32(msg->rule.dst_ip.addr),
	  ACL_IPV4_VAL_UINT32(msg->rule.dst_ip.mask));
	printf ("IP protocol %x %d %d\n", msg->rule.ip_protocol.valid,
	  msg->rule.ip_protocol.value, msg->rule.ip_protocol.mask);
	printf ("Src Port %x [%d %d-%d]\n",
	  msg->rule.src_protocol_port.valid,
	  msg->rule.src_protocol_port.oper,
	  msg->rule.src_protocol_port.starting_port_number,
	  msg->rule.src_protocol_port.ending_port_number);
	printf ("Dst Port %x [%d %d-%d]\n",
	  msg->rule.dst_protocol_port.valid,
	  msg->rule.dst_protocol_port.oper,
	  msg->rule.dst_protocol_port.starting_port_number,
	  msg->rule.dst_protocol_port.ending_port_number);
	printf ("IP TOS %x %d %d \n",msg->rule.tos.valid, msg->rule.tos.value,
	  msg->rule.tos.mask);
	printf ("TCP Flags %x urg(%d) ack(%d) push(%d) fin(%d) rst(%d) sync(%d)\n",
	  msg->rule.tcp_flags.valid,
	  msg->rule.tcp_flags.urg, msg->rule.tcp_flags.ack, msg->rule.tcp_flags.push,
	  msg->rule.tcp_flags.fin, msg->rule.tcp_flags.rst, msg->rule.tcp_flags.sync);

	printf ("Fwd Action %x \n", msg->rule.generic.results.forwarding);
	printf ("Sec Action %x \n", msg->rule.generic.results.supplemental);
	printf ("Res TOS Valid bit is %d \n",msg->rule.tos.valid);

	ruleid = (unsigned long long *) calloc(1, sizeof(unsigned long long));
	incr = incr + 1;
	*ruleid = incr;
	*out = ruleid;

	return (65538);
}
int ssm_dcm_bum_control_handler(const void *inp, void ** const out)
{
	printf ("Inside ssm_dcm_bum_control_handler \n \n");
	ssmBum_dcmd_config_t *msg = (ssmBum_dcmd_config_t *)inp;
	if (msg == NULL) 
	{
		printf ("No Message\n");
		return (-1);
	}
	printf ( "OPCODE = %d \n",msg->opcode);
	printf ( "PROTOCOL = %d \n",msg->protocol);
	printf ( "ACTION = %d \n",msg->action);
	printf ( "RATE FORMAT = %d \n",msg->rate_format);
	printf ( "RATE = %llu \n",msg->rate);
	printf ( "IFNAME = %s \n",(msg->if_name));

	return (65538);

}

int printAclRule(const void *inp, void ** const out)
{
	ssmAcl_dcmd_l2_rule_t *msg = (ssmAcl_dcmd_l2_rule_t *) inp;
	unsigned long long *ruleid = NULL;

	static unsigned long long incr = 0;

	if (msg == NULL) {
		printf ("Ruleid: %d doesn't exists\n", (uint32_t)((msg->rule).generic).seq_num);
		return (-1);
	}

	printf ("Opcode is %d \n", msg->opcode );

	printf ("Sequence number %d %u \n", msg->user_entered_sequence_number , (uint32_t)((msg->rule).generic).seq_num);
	printf ("Source Mac is %x "ACL_MAC_FMT_STR" "ACL_MAC_FMT_STR"  \n",msg->rule.src_mac.valid,
		ACL_MAC_VAL(msg->rule.src_mac.addr), ACL_MAC_VAL(msg->rule.src_mac.mask));
	printf ("Dest Mac is %x "ACL_MAC_FMT_STR" "ACL_MAC_FMT_STR"  \n",msg->rule.dst_mac.valid,
		ACL_MAC_VAL(msg->rule.dst_mac.addr), ACL_MAC_VAL(msg->rule.dst_mac.mask));
	printf ("EtherType is %x %u \n",msg->rule.ether_type.valid, msg->rule.ether_type.value);
	printf ("Fwd Action is %x \n",((((msg->rule).generic).results).forwarding));
	printf ("Sec Action is %x \n",((((msg->rule).generic).results).supplemental));

	ruleid = (unsigned long long *) calloc(1, sizeof(unsigned long long));
	incr = incr + 1;
	*ruleid = incr;
	*out = ruleid;

	return (65538);
}

int stormcontrol_dcm_clear_handler(const void *inp, void ** const out)
{
	printf ("Inside stormcontrol_dcm_clear_handler");
	ssmBum_dcmd_show_clear_t *msg = (ssmBum_dcmd_show_clear_t *) inp;
	printf ("Opcode is %d \n", msg->opcode );
	printf ("Ifname is %s \n", msg->if_name);
	printf ("Protocol is %d \n",msg->protocol);
	return (65538);
}

int stormcontrol_dcm_show_handler(const void *inp, void ** const out)
{
	ssmBum_dcmd_show_clear_t *msg = (ssmBum_dcmd_show_clear_t *) inp;
	printf ("Inside stormcontrol_dcm_show_handler\n ");

    printf ("Opcode is %d \n", msg->opcode );
    printf ("Ifname is %s \n", msg->if_name);
    printf ("Protocol is %d \n",msg->protocol);

    struct ssmBum_dcmd_status_response *resp
 				= (ssmBum_dcmd_status_response *)calloc (1, (sizeof(ssmBum_dcmd_status_response)));
	struct ssmBum_protocol_record *record = &resp->data.intf[0].protocol[0];

	(resp)->opcode = (ssmBum_dcmd_opcode_t)SSMBUM_DCMD_SHOW_SYSTEM_IF;
	strcpy ((resp->data).intf[0].if_name ,"Tengigabitethernet 11/0/20");
	(resp->data).num_intf = 1;
	(resp->data).intf[0].num_protocols = 1;
	resp->more_elements = 0;
	record->config.protocol = (ssmBum_protocol)1;
	record->config.provisioned_rate = 89;
	record->status.conforming = 1112;
	record->status.violations = 1113;

    *out = resp;

	return (65538);
}

int ssm_dcm_clear_handler(const void *inp, void ** const out)
{
	ssmAcl_dcmd_acl_show_request_t *msg = (ssmAcl_dcmd_acl_show_request_t *) inp;
	printf ("Opcode is %d \n", msg->opcode );
	printf ("Ifname is %s \n", msg->if_name);
	printf ("Acl name is %s \n", msg->acl_name);
	printf ("Direction is %d \n",msg->direction);
	printf ("Acl Type is %d \n",msg->type);
	return (65538);
}
int ssm_dcm_show_handler(const void *inp, void ** const out)
{

	struct ssmAcl_dcmd_status_response *resp = (ssmAcl_dcmd_status_response *)calloc (1, (sizeof(ssmAcl_dcmd_status_response) + sizeof(ssmAcl_rule_status)));

	printf ("Inside testapp\n ");

	for (size_t i =0; i <2; i++)
	{
		/* This is a case of mac std acl applied on one intf with two rule */
		(resp->resp_data).intf_rec[0].acl_rec[0].type = SSMACL_TYPE_L2_STD_E;
		strcpy ((resp->resp_data).intf_rec[0].acl_rec[0].name, "test");
		(resp->resp_data).intf_rec[0].acl_rec[0].direction = SSMACL_BINDING_DIRECTION_IN_E; 
		
		strcpy ((resp->resp_data).intf_rec[0].if_name ,"Te0/17");
		(resp->resp_data).intf_rec[0].acl_rec[0].direction = (ssmAcl_binding_direction) 1;
		strcpy(((resp)->resp_data).intf_rec[0].acl_rec[0].bind_src , "From CLI");
		
		(resp)->more_elements = 0;                                          
		(resp)->opcode = (ssmAcl_dcmd_opcode_t)SSMACL_DCMD_SHOW_ACCESSLIST_ALL;
		
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.num_of_rules = 2;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.generic.seq_num = (uint32_t)12;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.generic.results.forwarding 
			= (ssmAcl_forwarding_result)SSMACL_FORWARDING_RESULT_PERMIT_E;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.src_mac.valid = 1;
		if (i == 0)
			((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.dst_mac.valid = 1;
		else
			((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.dst_mac.valid = 0;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.ether_type.valid = 1;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].l2_rule.ether_type.value = 2000;
		((resp)->resp_data).intf_rec[0].acl_rec[0].data.rule[i].hw_programming = (ssmAcl_binding_status)1;
	}

	*out = resp;

	return (65538);
}

int printAclPolicy(const void *inp, void ** const out)
{
    ssmAcl_dcmd_acl_t *msg = (ssmAcl_dcmd_acl_t *) inp;

    if (msg == NULL) {
        return (-1);
    }

    printf("Opcode is %d \n", msg->opcode );
    printf("PolicyType is %d \n", msg->type);
    printf("PolicyName is %s \n", msg->acl_name);
    printf("Intf Name is %s \n", msg->if_name);
    printf("Direction is %d \n", msg->direction);

    /* This is mandatory. If you have structure assing the valid memory
     * otherwise assign null
     */
    *out = NULL;

    return (65538);

}

int printAclBulkIntfAssoc(const void *inp, void ** const out)
{
	L2aclIntfAssocBulkMsg_t *pmsg = (L2aclIntfAssocBulkMsg_t *)inp;
	L2aclPolicyMsg_t *msg;
	unsigned long long i;
	if (pmsg == NULL)
		return (-1);

	printf("Input Num Records:%llu\n", pmsg->numrecords);

	for (i = 0; i < pmsg->numrecords; i++) {
		msg = (L2aclPolicyMsg_t *)&(pmsg->intflist[i]);
		printf("Input PolicyName: %s\n", msg->policyname);
		printf("Input Interface: %s\n", msg->ifname);
		printf("Input opcode: %d\n", msg->opcode);
	}

	*out = NULL;

	return (65538);
}

int printCeeMap(const void *inp, void ** const out)
{
    QosCeeMapMsg_t *msg_p = (QosCeeMapMsg_t *)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return 0;
    }

    printf("ceeMapName=%s, precedence=%d\n", msg_p->ceeMapName, msg_p->precedence);
    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int printPriorityGroup(const void *inp, void ** const out)
{
    QosPriorityGroupMsg_t *msg_p = (QosPriorityGroupMsg_t*)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return WRC_WAVE_MESSAGE_SUCCESS;
    }

    printf("ceeMapName=%s, pgId=%s, weight=%d, pfc=%d\n",
            msg_p->ceeMapName,
            msg_p->pgid,
            msg_p->weight,
            msg_p->pfc);

    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int printPriorityMap(const void *inp, void ** const out)
{
    QosPriorityMapMsg_t *msg_p = (QosPriorityMapMsg_t*)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return WRC_WAVE_MESSAGE_SUCCESS;
    }

    printf("ceeMapName=%s, pgId0=%s, pgId1=%s, pgId2=%s, pgId3=%s, pgId4=%s, pgId5=%s, pgId6=%s, pgId7=%s, \n",
            msg_p->ceeMapName,
            msg_p->pgid0,
            msg_p->pgid1,
            msg_p->pgid2,
            msg_p->pgid3,
            msg_p->pgid4,
            msg_p->pgid5,
            msg_p->pgid6,
            msg_p->pgid7);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printCeeRemap(const void *inp, void ** const out)
{
    QosCeeRemapMsg_t *msg_p = (QosCeeRemapMsg_t *)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return WRC_WAVE_MESSAGE_SUCCESS;
    }

    printf("ceeMapName=%s, remapPriroity=%d, remapFabricP=%d\n",
            msg_p->ceeMapName,
            msg_p->remapPriority,
            msg_p->remapFabricP);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printCee(const void *inp, void ** const out)
{
    qos_interface_config_msg_t *msg = (qos_interface_config_msg_t*)inp;

    if (msg == NULL)
        return (-1);

    switch(msg->op_code) {
        case (CONFIGURE_INTERFACE_QOS):
        {
            switch (msg->cmd_code) {
                case (QOS_INTERFACE_CEE_MAP):
                    printf("ifname %s, ceeMap %s\n", msg->ifname, msg->cee_map);
                    break;
                default:
                    break;
            }
            break;
        }
        case (DELETE_INTERFACE_QOS):
        {
            switch (msg->cmd_code) {
                case (QOS_INTERFACE_CEE_MAP):
                    printf("ifname %s\n", msg->ifname);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int dummy(const void *inp, void ** const out)
{
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int qosProfileHandler( const void *inp, void ** const out) {
    printf("TestApp.c :: qosProfileHandler() ... \n");
    printf("\n\n");
    *out=NULL;
    return(0);
}


int secProfileHandler( const void *inp, void ** const out) {
    printf("TestApp.c :: secProfileHandler() ... \n");
    printf("\n\n");
    *out=NULL;
    return(0);
}


int fcoePHandler( const void *inp, void ** const out ) {
    printf("TestApp.c :: fcoePHandler() ... \n");
    printf("\n\n");
    *out = NULL ;
    return (0);
}

int xstp_config_handler (const void *inp, void ** const out) {
    mstp_msg_port_config *in = (mstp_msg_port_config*) inp;
    if (in == NULL) {
        printf("ssm_app.c :: xstp_config_handler() : NULL input\n");
        return -1;
    }

    printf("ssm_app.c :: xstp_config_handler() : ifname=%s, cmd=%u, bpdu-drop=%d, tagged-ieee-bpdu=%d\n",
                in->name, 
                in->cmd, 
                in->bpdu_guard_enable, 
                in->tagged_bpdu_enable
                );
    *out = NULL;
    return 0;
}


int main(int argc, char **argv)
{
    int ret = 0;

    int mgmt_port = 9109;

    if (argc != 2) {
    	printf("usage: ssm_app.Linux.i686 mgmt_port\n");
    	return 0;
	}

	mgmt_port += atoi(argv[1]);


    /* STEP 2 */
    setSSMMsgHandlerForClient();

    /* STEP 3 */
	add_client_message_handler(MSG_STORMCONTROL_PHY_INTF_CONF, ssm_dcm_bum_control_handler);
    add_client_message_handler(STORMCONTROLCLIENTCLEAR, stormcontrol_dcm_clear_handler);
    add_client_message_handler(STORMCONTROLCLIENTSHOW, stormcontrol_dcm_show_handler);
	add_client_message_handler(SSMMACIPACLCLEAR, ssm_dcm_clear_handler);
	add_client_message_handler(SSMMACIPACLSHOW, ssm_dcm_show_handler);
    add_client_message_handler(SSMMACACLPOLICY, printAclPolicy);
    add_client_message_handler(SSMMACACLRULE, printAclRule);
    add_client_message_handler(SSMMACACLBULKPOLICY, printAclBulkPolicy);
    add_client_message_handler(SSMMACACLINTFASSOCBULK, printAclBulkIntfAssoc);
    add_client_message_handler(SSMIPACLRULE,  printIPv4AclRule);
	add_client_message_handler(SSMIPACLPOLICY, printIPv4AclPolicy);
	add_client_message_handler(SSMIPACLRESEQUENCE, printAclResequence);
	add_client_message_handler(SSMMACACLRESEQUENCE, printAclResequence);
	add_client_message_handler(QOSCLIENTCREATECEEMAP, printCeeMap);
    add_client_message_handler(QOSCLIENTCREATEPRIORITYGROUP, printPriorityGroup);
    add_client_message_handler(QOSCLIENTCREATEPRIORITYMAP, printPriorityMap);
    add_client_message_handler(QOSCLIENTCREATECEEREMAP, printCeeRemap);
//    add_client_message_handler(QOSCLIENTDELETECEEMAP, printCeeMap);
    add_client_message_handler(QOSCLIENTDELETEPRIORITYGROUP, printPriorityGroup);
    add_client_message_handler(QOSCLIENTDELETEPRIORITYMAP, dummy);
    add_client_message_handler(SSM_CLIENT_ADD_ROUTEMAP, dummy);
    add_client_message_handler(SSM_CLIENT_DEL_ROUTEMAP, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_NH, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_ADDR, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_METRIC, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TYPE, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TAG, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_SET_DISTANCE, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_SET_METRIC, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TYPE, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TAG, dummy);
    add_client_message_handler(SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH, dummy);
    add_client_message_handler(SSM_CLIENT_ADD_IPPREFIX, dummy);
    add_client_message_handler(SSM_CLIENT_DEL_IPPREFIX, dummy);

    // Register the message Handlers for QOS and Security Profiles.
    add_client_message_handler( APPMQOSPROFILE, qosProfileHandler );
    add_client_message_handler( APPMSECPROFILE, secProfileHandler );

    //Fcoe message handler is added to both nsm and ssm
    add_client_message_handler(APPMFCOEPROFILE, fcoePHandler);

    // xSTP specific handlers for SSM
    add_client_message_handler(MSTPPORTCONFIG, xstp_config_handler);


	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);


    /* STEP 4: Initialize Dcmd */
    ret = dcm_initialize_client ("ssm");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from ssm client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from ssm client... \n");
    } 

    while(1)
        sleep(100);
}
