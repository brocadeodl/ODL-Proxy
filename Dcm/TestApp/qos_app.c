/*
 *Application to handle the messages dispatched to the qos daemon.
 *The file has message handlers for
 * 1. classical qos commands.
 * 2. cee commands.
 * 3. flow-based qos commands.
 *
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "PhysicalInterface/ClientInterface/PhysicalInterfaceManagementInterfaceMessagesHandler.h"
#include "ValClientInterface/DcmClientInterface.h"
#include <assert.h>
#include "ClientInterface/Ssm/QosClientInterface.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include <assert.h>
#include "ssm_policer_dcmd_public.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "qosd_fb_public.h"
#include "ssmAcl_public.h"
#include "ClientInterface/Qos/qos_fb_dcm_common.h"

#include "ClientInterface/Ssm/qoscommon.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "WaveCStatus.h"
#include "DcmCStatus.h"

int printCeeMap(const void *inp, void ** const out) {
	QosCeeMapMsg_t *msg_p = (QosCeeMapMsg_t *) inp;

	if (msg_p == NULL) {
		printf("msg is NULL");
		return WAVE_MESSAGE_SUCCESS;
	}

	printf("ceeMapName=%s, precedence=%d\n", msg_p->ceeMapName,
			msg_p->precedence);
	return (WAVE_MESSAGE_SUCCESS);
}

int printPriorityGroup(const void *inp, void ** const out) {
	QosPriorityGroupMsg_t *msg_p = (QosPriorityGroupMsg_t*) inp;

	if (msg_p == NULL) {
		printf("msg is NULL");
		return WAVE_MESSAGE_SUCCESS;
	}

	printf("ceeMapName=%s, pgId=%s, weight=%d, pfc=%d\n", msg_p->ceeMapName,
			msg_p->pgid, msg_p->weight, msg_p->pfc);

	return (WAVE_MESSAGE_SUCCESS);
}

int printPriorityMap(const void *inp, void ** const out) {
	QosPriorityMapMsg_t *msg_p = (QosPriorityMapMsg_t*) inp;

	if (msg_p == NULL) {
		printf("msg is NULL");
		return WAVE_MESSAGE_SUCCESS;
	}

	printf(
			"ceeMapName=%s, pgId0=%s, pgId1=%s, pgId2=%s, pgId3=%s, pgId4=%s, pgId5=%s, pgId6=%s, pgId7=%s, \n",
			msg_p->ceeMapName, msg_p->pgid0, msg_p->pgid1, msg_p->pgid2,
			msg_p->pgid3, msg_p->pgid4, msg_p->pgid5, msg_p->pgid6,
			msg_p->pgid7);

	return WAVE_MESSAGE_SUCCESS;
}

int printCeeRemap(const void *inp, void ** const out) {
	QosCeeRemapMsg_t *msg_p = (QosCeeRemapMsg_t *) inp;

	if (msg_p == NULL) {
		printf("msg is NULL");
		return WAVE_MESSAGE_SUCCESS;
	}

	printf("ceeMapName=%s, remapPriroity=%d, remapFabricP=%d\n",
			msg_p->ceeMapName, msg_p->remapPriority, msg_p->remapFabricP);

	return WAVE_MESSAGE_SUCCESS;
}

int printCee(const void *inp, void ** const out) {
	qos_interface_config_msg_t *msg = (qos_interface_config_msg_t*) inp;

	if (msg == NULL)
		return (-1);

	switch (msg->op_code) {
	case (CONFIGURE_INTERFACE_QOS): {
		switch (msg->cmd_code) {
		case (QOS_INTERFACE_CEE_MAP):
			printf("ifname %s, ceeMap %s\n", msg->ifname, msg->cee_map);
			break;
		default:
			break;
		}
		break;
	}
	case (DELETE_INTERFACE_QOS): {
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
	return (WAVE_MESSAGE_SUCCESS);
}

int dummy(const void *inp, void ** const out) {
	return WAVE_MESSAGE_SUCCESS;
}

int PolicerClassMapMessageHandler(const void *inp, void ** const out) {
	fbDcmClassMapMessage_t *msg = (fbDcmClassMapMessage_t *) inp;

	if (msg->opcode == FB_CLASSMAP_CREATE)
		printf("create class map");
	else if (msg->opcode == FB_CLASSMAP_DELETE)
		printf("delete class map");
	else
		printf("unrecognized opcode");

	printf("Opcode : %d\n", (msg->opcode));
	printf("class map name: %s\n", (msg->name));
	printf("match criteria : %d\n", msg->rule);
	printf("Access List Name : %s\n", msg->accessListName);

	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int PolicerPolicyClassMapMessageHandler(const void *inp, void ** const out) {
	fbDcmCommandCode_t *opcode = (fbDcmCommandCode_t *) inp;

	if ((*opcode == FB_POLICYMAP_CREATE) || (*opcode == FB_POLICYMAP_DELETE)) {
		fbDcmPolicyClassMapActionMessage_t *msg =
				(fbDcmPolicyClassMapActionMessage_t *) inp;
		printf("opcode: %d\n", msg->opcode);
		printf("policy map name: %s\n", ((msg->policy_name)));
	} else if ((*opcode == FB_ADD_CLASS_MAP_TO_POLICY_MAP)
			|| (*opcode == FB_DEL_CLASS_MAP_TO_POLICY_MAP)) {
		fbDcmPolicyClassMapActionMessage_t *msg =
				(fbDcmPolicyClassMapActionMessage_t *) inp;
		printf("opcode: %d\n", msg->opcode);
		printf("policy map name: %s\n", ((msg->policy_name)));
		printf("class map name: %s\n", ((msg->class_name.name)));
	} else {
		fbDcmPolicyClassMapActionMessage_t *msg =
				(fbDcmPolicyClassMapActionMessage_t *) inp;
		printf("opcode: %d\n", msg->opcode);

		printf("policy map name: %s\n", ((msg->policy_name)));
		printf("class map name: %s\n", ((msg->class_name).name));

		//Print policer attributes.
		printf("cir: %d\n", (int) ((msg->action.police_attr).cir));
		printf("cbs: %d\n", (int) ((msg->action.police_attr).cbs));
		printf("eir: %d\n", (int) ((msg->action.police_attr).eir));
		printf("ebs: %d\n", (int) ((msg->action.police_attr).ebs));
		printf("conform_ip_prec: %d\n",
				(int) ((msg->action.police_attr).conform_ip_tos));
		printf("conform_tc: %d\n",
				(int) ((msg->action.police_attr).conform_traffic_class));
		printf("exceed_ip_prec: %d\n",
				(int) ((msg->action.police_attr).exceed_ip_tos));
		printf("exceed_tc: %d\n",
				(int) ((msg->action.police_attr).exceed_traffic_class));
		printf("conform_ip_prec bit: %d\n",
				(int) ((msg->action.police_attr).set_conform_ip_prec));
		printf("conform_dscp bit: %d\n",
				(int) ((msg->action.police_attr).set_conform_dscp));
		printf("exceed_ip_prec bit: %d\n",
				(int) ((msg->action.police_attr).set_exceed_ip_prec));
		printf("exceed_dscp bit: %d\n",
				(int) ((msg->action.police_attr).set_exceed_dscp));
		printf("priority map: %s\n", (msg->action.police_attr).prio_map_name);

		//Print set cos and tc
		printf("cos value : %d\n", (int) (msg->action.cos_attr.cos));
		printf("tc value : %d\n", (int) (msg->action.cos_attr.tc));
		printf("dscp value : %d\n", (int) (msg->action.dscp_attr.dscp));

		//Print Shaping Rate Values
		printf("Shaping rate value : %d\n",
				(int) (msg->action.portshape_attr.shape_rate));

		//print qos maps
		printf("cosmutation map name : %s\n",
				(msg->action.cosmutation_attr.map_name));
		printf("cos traffic class map name : %s\n",
				(msg->action.costrafficclass_attr.map_name));
		printf("dscp mutation map name : %s\n",
				(msg->action.dscpmutation_attr.map_name));
		printf("dscp cos map name : %s\n", (msg->action.dscpcos_attr.map_name));
		printf("dscp traffic class map name : %s\n",
				(msg->action.dscptrafficclass_attr.map_name));
		printf("cee map name : %s\n", (msg->action.cee_attr.map_name));

	}

	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int PolicerTeMessageHandler(const void *inp, void ** const out) {
	ssm_pol_bind_policymap_msg_t *pInput = (ssm_pol_bind_policymap_msg_t *) inp;

	printf("map name %s\n", pInput->policy_map_name.name);
	printf("int name %s\n", pInput->if_name);
	printf("opcode %d\n", pInput->opcode);
	printf("cmdcode %d\n", pInput->cmdcode);
	return (WAVE_MESSAGE_SUCCESS);
}

int PolicerPriorityMapMessageHandler(const void *inp, void ** const out) {
	ssm_pol_prio_map_msg_t *msg = (ssm_pol_prio_map_msg_t *) inp;

	printf("Input priority map name: %s\n", (msg->prio_map).name);

	printf("Input priority opcode: %d\n", msg->opcode);

	if (msg->opcode == SSM_POL_PRIORITYMAP_CREATE) {
		printf("Conform0 %d\n", (msg->prio_map).conform_cos[0]);
		printf("Conform1 %d\n", (msg->prio_map).conform_cos[1]);
		printf("Conform2 %d\n", (msg->prio_map).conform_cos[2]);
		printf("Conform3 %d\n", (msg->prio_map).conform_cos[3]);
		printf("Conform4 %d\n", (msg->prio_map).conform_cos[4]);
		printf("Conform5 %d\n", (msg->prio_map).conform_cos[5]);
		printf("Conform6 %d\n", (msg->prio_map).conform_cos[6]);
		printf("Conform7 %d\n", (msg->prio_map).conform_cos[7]);

		printf("Exceed0 %d\n", (msg->prio_map).exceed_cos[0]);
		printf("Exceed1 %d\n", (msg->prio_map).exceed_cos[1]);
		printf("Exceed2 %d\n", (msg->prio_map).exceed_cos[2]);
		printf("Exceed3 %d\n", (msg->prio_map).exceed_cos[3]);
		printf("Exceed4 %d\n", (msg->prio_map).exceed_cos[4]);
		printf("Exceed5 %d\n", (msg->prio_map).exceed_cos[5]);
		printf("Exceed6 %d\n", (msg->prio_map).exceed_cos[6]);
		printf("Exceed7 %d\n", (msg->prio_map).exceed_cos[7]);

		printf("ConformCos %d\n", (msg->prio_map).set_conform_cos);
		printf("ExceedCos %d\n", (msg->prio_map).set_exceed_cos);
	}

	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int PolicerShowMessageHandler(const void *inp, void ** const out) {
	fbDcmShowPmapIntInputMessage_t *pInput =
			(fbDcmShowPmapIntInputMessage_t *) inp;

	printf("interface %s\n", pInput->if_name);
	printf("direction %d\n", pInput->direction);
	printf("cmdcode %d\n", pInput->opcode);

	if (pInput->opcode == FB_SHOW_POLICYMAP_ALL) {
		ssm_pol_show_stats_summary_msg_t *payload_p = NULL;

		size_t size;

		size = sizeof(ssm_pol_show_stats_summary_msg_t);

		payload_p = (ssm_pol_show_stats_summary_msg_t *) malloc(size);

		payload_p->num_records = 1;
		payload_p->data_size = size;
		strcpy(payload_p->record[0].interface, "te0/1");
		strcpy(payload_p->record[0].policy_map_name_ingress.name, "inmap");
		memset(payload_p->record[0].policy_map_name_egress.name, 0,
				sizeof(payload_p->record[0].policy_map_name_egress.name));

		*out = payload_p;
	} else {
		ssm_pol_show_stats_detail_msg_t *payload_p = NULL;

		size_t size;

		size = sizeof(ssm_pol_show_stats_detail_msg_t)
				+ sizeof(ssm_pol_show_stats_data_t)
				+ sizeof(ssm_pol_class_map_stats_data_t);

		payload_p = (ssm_pol_show_stats_detail_msg_t *) malloc(size);

		payload_p->num_records = 2;
		payload_p->data_size = size;
		payload_p->records[0].num_class_maps = 1;
		payload_p->records[1].num_class_maps = 2;
		//payload_p->records[1].stats_data[0].policer_attr.cir = POLICER_NOT_USR_CFGED_CIR_CBS;
		payload_p->records[1].stats_data[0].policer_attr.eir =
				POLICER_NOT_USR_CFGED_CIR_CBS;
		payload_p->records[1].stats_data[0].policer_attr.conform_ip_tos = 64;
		strncpy(payload_p->records[0].stats_data[0].class_map_name.name, "cmap",
				68);

		strncpy(payload_p->records[0].interface, "te0/1", 68);

		strncpy(payload_p->records[1].stats_data[0].class_map_name.name, "zmap",
				68);

		strncpy(payload_p->records[1].interface, "te0/2", 68);

		strncpy(payload_p->records[1].stats_data[1].class_map_name.name,
				"z1map", 68);

		*out = payload_p;
	}
	//*out = NULL;

	return (0);
}

int PolicerClearMessageHandler(const void *inp, void ** const out) {
	ssm_pol_show_or_clr_input_msg_t *pInput =
			(ssm_pol_show_or_clr_input_msg_t *) inp;

	//printf("interface name %s\n", pInput->if_name);
	printf("interface %s\n", pInput->interface);
	//printf("policy name %s\n", pInput->map_name);
	printf("direction %d\n", pInput->direction);
	printf("cmdcode %d\n", pInput->cmdcode);
	return (0);
}

int PolicerDebugMessageHandler(const void *inp, void ** out) {

	printf("reaching backend\n");

	*out = NULL;
	return (0);
}

int createCosToCosMutationMap(const void *inp, void ** const out) {
	QosCosToCosMsg_t *msg = (QosCosToCosMsg_t*) inp;
	printf("Input cosToCosMutationMapName: %s\n", msg->cosToCosMutationMapName);
	printf("Input cos0MutatedCosValue: %d\n", msg->cos0MutatedCosValue);
	printf("Input cos1MutatedCosValue: %d\n", msg->cos1MutatedCosValue);
	printf("Input cos2MutatedCosValue: %d\n", msg->cos2MutatedCosValue);
	printf("Input cos3MutatedCosValue: %d\n", msg->cos3MutatedCosValue);
	printf("Input cos4MutatedCosValue: %d\n", msg->cos4MutatedCosValue);
	printf("Input cos5MutatedCosValue: %d\n", msg->cos5MutatedCosValue);
	printf("Input cos6MutatedCosValue: %d\n", msg->cos6MutatedCosValue);
	printf("Input cos7MutatedCosValue: %d\n", msg->cos7MutatedCosValue);

	*out = NULL;

	return (WAVE_MESSAGE_SUCCESS);
}

int createCosToTrafficClassMap(const void *inp, void ** const out) {
	QosCosToTrafficClassMsg_t *msg = (QosCosToTrafficClassMsg_t*) inp;
	printf("Input cosToTrafficClassMapName: %s\n",
			msg->cosToTrafficClassMapName);
	printf("Input cos0MappedTrafficClassValue: %d\n",
			msg->cos0MappedTrafficClassValue);
	printf("Input cos1MappedTrafficClassValue: %d\n",
			msg->cos1MappedTrafficClassValue);
	printf("Input cos2MappedTrafficClassValue: %d\n",
			msg->cos2MappedTrafficClassValue);
	printf("Input cos3MappedTrafficClassValue: %d\n",
			msg->cos3MappedTrafficClassValue);
	printf("Input cos4MappedTrafficClassValue: %d\n",
			msg->cos4MappedTrafficClassValue);
	printf("Input cos5MappedTrafficClassValue: %d\n",
			msg->cos5MappedTrafficClassValue);
	printf("Input cos6MappedTrafficClassValue: %d\n",
			msg->cos6MappedTrafficClassValue);
	printf("Input cos7MappedTrafficClassValue: %d\n",
			msg->cos7MappedTrafficClassValue);

	*out = NULL;

	return (WAVE_MESSAGE_SUCCESS);
}

int createMcastScheduler(const void *inp, void ** const out) {
	QosMcastSchedulerMsg_t *msg = (QosMcastSchedulerMsg_t*) inp;
	printf("Input dwrrTrafficClass0=%d\n", msg->dwrrTrafficClass0);
	printf("Input dwrrTrafficClass1=%d\n", msg->dwrrTrafficClass1);
	printf("Input dwrrTrafficClass2=%d\n", msg->dwrrTrafficClass2);
	printf("Input dwrrTrafficClass3=%d\n", msg->dwrrTrafficClass3);
	printf("Input dwrrTrafficClass4=%d\n", msg->dwrrTrafficClass4);
	printf("Input dwrrTrafficClass5=%d\n", msg->dwrrTrafficClass5);
	printf("Input dwrrTrafficClass6=%d\n", msg->dwrrTrafficClass6);
	printf("Input dwrrTrafficClass7=%d\n", msg->dwrrTrafficClass7);
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int deleteMcastScheduler(const void *inp, void ** const out) {
	printf("PRINTING --- inside deleteMcastScheduler\n");
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int createScheduler(const void *inp, void ** const out) {
	QosSchedulerMsg_t *msg = (QosSchedulerMsg_t*) inp;
	printf("Input strict_priority=%d\n", msg->strict_priority);
	printf("Input dwrrTrafficClass0=%d\n", msg->dwrrTrafficClass0);
	printf("Input dwrrTrafficClass1=%d\n", msg->dwrrTrafficClass1);
	printf("Input dwrrTrafficClass2=%d\n", msg->dwrrTrafficClass2);
	printf("Input dwrrTrafficClass3=%d\n", msg->dwrrTrafficClass3);
	printf("Input dwrrTrafficClass4=%d\n", msg->dwrrTrafficClass4);
	printf("Input dwrrTrafficClass5=%d\n", msg->dwrrTrafficClass5);
	printf("Input dwrrTrafficClass6=%d\n", msg->dwrrTrafficClass6);
	printf("Input dwrrTrafficClass7=%d\n", msg->dwrrTrafficClass7);
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int deleteScheduler(const void *inp, void ** const out) {
	printf("PRINTING --- inside deleteScheduler\n");
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int createMcastSetting(const void *inp, void ** const out) {
	QosRcvQueueMulticastMsg_t *msg = (QosRcvQueueMulticastMsg_t*) inp;
	printf("Input opCode=%d\n", msg->opCode);
	printf("Input thresholdTrafficClass0=%d\n", msg->thresholdTrafficClass0);
	printf("Input thresholdTrafficClass1=%d\n", msg->thresholdTrafficClass1);
	printf("Input thresholdTrafficClass2=%d\n", msg->thresholdTrafficClass2);
	printf("Input thresholdTrafficClass3=%d\n", msg->thresholdTrafficClass3);
	printf("Input thresholdTrafficClass4=%d\n", msg->thresholdTrafficClass4);
	printf("Input thresholdTrafficClass5=%d\n", msg->thresholdTrafficClass5);
	printf("Input thresholdTrafficClass6=%d\n", msg->thresholdTrafficClass6);
	printf("Input thresholdTrafficClass7=%d\n", msg->thresholdTrafficClass7);
	printf("Input rateLimit=%d\n", msg->rateLimit);
	printf("Input burst=%d\n", msg->burst);
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int deleteMcastSetting(const void *inp, void ** const out) {
	printf("PRINTING --- inside deleteMcastSetting\n");
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int qosClearCounters(const void *inp, void ** const out) {
	printf("PRINTING --- inside clear counters\n");
	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}
int deleteCosToCosMutationMap(const void *inp, void ** const out) {
	char *msg = (char*) inp;
	printf("Input cosToCosMapName: %s\n", msg);
	*out = NULL;

	return (WAVE_MESSAGE_SUCCESS);
}

int deleteCosToTrafficClassMap(const void *inp, void ** const out) {
	char *msg = (char*) inp;
	printf("Input cosToTrafficClassMapName: %s\n", msg);
	*out = NULL;

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateDscpMutationMap(const void *inp, void ** const out) {
	QosDscpMutationMapMsg_t *msg = (QosDscpMutationMapMsg_t *) inp;

	printf("Qos Test App...got Create Dscp Mutation Map..\n");
	printf("Map name: %s\n", msg->dscpMutationMapName);
	*out = NULL;

	return (WAVE_MESSAGE_SUCCESS);
}

int qosDeleteDscpMutationMap(const void *inp, void ** const out) {
	QosDscpMutationMapMsg_t *msg = (QosDscpMutationMapMsg_t *) inp;

	printf("Qos Test App...got Delete Dscp Mutation Map..\n");
	printf("Map name: %s\n", msg->dscpMutationMapName);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateDscpTrafficClassMap(const void *inp, void ** const out) {
	QosDscpTrafficClassMapMsg_t *msg = (QosDscpTrafficClassMapMsg_t *) inp;

	printf("Qos Test App...got Create Dscp Traffic class map..\n");
	printf("Map Name: %s\n", msg->dscpTrafficClassMapName);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosDeleteDscpTrafficClassMap(const void *inp, void ** const out) {
	QosDscpTrafficClassMapMsg_t *msg = (QosDscpTrafficClassMapMsg_t *) inp;

	printf("Qos Test App...got Delete Dscp Traffic class map..\n");
	printf("Map Name: %s\n", msg->dscpTrafficClassMapName);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateDscpCosMap(const void *inp, void ** const out) {
	QosDscpCosMapMsg_t *msg = (QosDscpCosMapMsg_t *) inp;

	printf("Qos Test App...got Create Dscp CoS class map..\n");
	printf("Map Name: %s\n", msg->dscpCosMapName);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosDeleteDscpCosMap(const void *inp, void ** const out) {
	QosDscpCosMapMsg_t *msg = (QosDscpCosMapMsg_t *) inp;

	printf("Qos Test App...got Delete Dscp CoS class map..\n");
	printf("Map Name: %s\n", msg->dscpCosMapName);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateRedProfile(const void *inp, void ** const out) {
	QosRedProfileIdsMsg_t *msg = (QosRedProfileIdsMsg_t *) inp;

	printf("Qos Test App...got Create RED profile..\n");
	printf("Profile Id: %d\n", msg->profileId);
	printf("minThreshold: %d\n", msg->minThreshold);
	printf("maxThreshold: %d\n", msg->maxThreshold);
	printf("dropProbability: %d\n", msg->dropProbability);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosDeleteRedProfile(const void *inp, void ** const out) {
	QosRedProfileIdsMsg_t *msg = (QosRedProfileIdsMsg_t *) inp;

	printf("Qos Test App...got Delete RED profile..\n");
	printf("Profile Id: %d\n", msg->profileId);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateDscpMarkList(const void *inp, void ** const out) {
	QosDscpMarkListMsg_t *msg = (QosDscpMarkListMsg_t *) inp;
	uint32_t i;

	printf("Qos Test App...got create mark List message..\n");
	printf("Qos Map name: %s\n", msg->dscpMapName);
	printf("Qos Map Type: %d\n", msg->qosMapType);

	printf("Map: \n");
	for (i = 0; i < msg->nValues; i++) {
		printf(" %5d", msg->pValues[i]);
	}

	printf(" to %5d\n", msg->toValue);

	return (WAVE_MESSAGE_SUCCESS);
}

int qosCreateCeeReMap(const void *inp, void ** const out) {
	printf("Reached Here\n");

	return (WAVE_MESSAGE_SUCCESS);
}

int qosClientConfigIntf(const void *inp, void ** const out) {
	printf("PRINTING  ---- inside qosClientConfigIntf\n");
	qos_interface_config_msg_t *msg = (qos_interface_config_msg_t*) inp;
	printf("PRINTING --- msg->ifname=%s\n", msg->ifname);
	printf("PRINTING --- msg->cmd_code =%d\n", msg->cmd_code);
	printf("PRINTING --- msg->op_code =%d\n", msg->op_code);
	switch (msg->op_code) {
	case CONFIGURE_INTERFACE_QOS: {
		printf("PRINTING --- wave client CONFIGURE_INTERFACE_QOS\n");
		switch (msg->cmd_code) {
		case QOS_INTERFACE_TRUST_COS:
			printf("PRINTING -- wave client QOS_INTERFACE_TRUST_COS\n");
			break;
		case QOS_INTERFACE_TRUST_DSCP:
			printf("PRINTING -- wave client QOS_INTERFACE_TRUST_DSCP\n");
			break;
		case QOS_INTERFACE_DEFAULT_COS:
			printf("PRINTING --- wave client QOS_INTERFACE_DEFAULT_COS=%d\n",
					msg->default_cos);
			break;
		case QOS_INTERFACE_FLOW_CONTROL_TX:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_FLOW_CONTROL_TX=%u\n",
					msg->flowcontrol_tx);
			break;
		case QOS_INTERFACE_FLOW_CONTROL_RX:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_FLOW_CONTROL_RX=%u\n",
					msg->flowcontrol_rx);
			break;
		case QOS_INTERFACE_FLOW_CONTROL_BOTH:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_FLOW_CONTROL_TX=%u\n",
					msg->flowcontrol_tx);
			printf(
					"PRINTING --- wave client QOS_INTERFACE_FLOW_CONTROL_RX=%u\n",
					msg->flowcontrol_rx);
			break;
		case QOS_INTERFACE_COS_THRESHOLD:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_COS_THRESHOLD: %d %d %d"
							" %d %d %d %d %d\n", msg->cos_threshold[0],
					msg->cos_threshold[1], msg->cos_threshold[2],
					msg->cos_threshold[3], msg->cos_threshold[4],
					msg->cos_threshold[5], msg->cos_threshold[6],
					msg->cos_threshold[7]);
			break;
		case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH tx=%u\n",
					msg->pfc_flowcontrol_tx);
			printf(
					"PRINTING --- wave client QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH rx=%u\n",
					msg->pfc_flowcontrol_rx);
			printf(
					"PRINTING --- wave client QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH pfc-cos=%d\n",
					msg->pfc_cos);
			break;
		case QOS_INTERFACE_COS_PROFILE:
			printf("PRINTING --- wave client QOS_INTERFACE_COS_PROFILE=%u\n",
					msg->red_profile);
			break;
		case QOS_INTERFACE_TAIL_DROP:
			printf("PRINTING --- wave client QOS_INTERFACE_TAIL_DROP=%u\n",
					msg->tail_drop);
			break;
		case QOS_INTERFACE_DSCP_TRAFFIC_CLASS:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_DSCP_TRAFFIC_CLASS=%s\n",
					msg->dscp_traffic_class_map);
			break;
		case QOS_INTERFACE_DSCP_MUTATION:
			printf("PRINTING --- wave client QOS_INTERFACE_COS_MUTATION=%s\n",
					msg->dscp_mutation_map);
			break;
		case QOS_INTERFACE_DSCP_COS:
			printf("PRINTING --- wave client QOS_INTERFACE_DSCP_COS=%s\n",
					msg->dscp_cos_map);
			break;
		case QOS_INTERFACE_COS_MUTATION:
			printf("PRINTING --- wave client QOS_INTERFACE_COS_MUTATION=%s\n",
					msg->cos_mutation_map);
			break;
		case QOS_INTERFACE_COS_TRAFFIC_CLASS:
			printf(
					"PRINTING --- wave client QOS_INTERFACE_COS_TRAFFIC_CLASS=%s\n",
					msg->cos_traffic_class_map);
			break;

		}
		break;
	}
	case DELETE_INTERFACE_QOS:
		printf("PRINTING --- wave client DELETE_INTERFACE_QOS\n");
		switch (msg->cmd_code) {
		case QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH:
			printf(
					"PRINTING ---- wave client QOS_INTERFACE_PFC_FLOW_CONTROL_BOTH=%d\n",
					msg->pfc_cos);
			break;
		case QOS_INTERFACE_COS_THRESHOLD:
			printf("PRINTING ---- wave client QOS_INTERFACE_COS_THRESHOLD\n");
			break;
		}

		break;
	}

	*out = NULL;
	return (WAVE_MESSAGE_SUCCESS);
}

int QosClientNasHandler (const void *inp, void ** const out) {
    printf("Inside QosClientNasHandler \n");
    qosNasDcmdConfigMsg_t *msg = (qosNasDcmdConfigMsg_t*) inp;
    printf("Nas opcode = %u\n", msg->opcode);
    printf("Nas cmdcode = %u\n", msg->cmdcode);
    printf("Nas cos= %u\n", msg->cos);
    printf("Nas dscp= %u\n", msg->dscp);
    printf("Nas ipaddr= %u\n", msg->ip_addr);
    printf("Nas mask= %u\n", msg->mask);
    printf("Nas vlannumber= %u\n", msg->vlan_number);
    printf("Nas vrf-name= %s\n", msg->vrf_name);


    return (WAVE_MESSAGE_SUCCESS);
}


int main(int argc, char **argv) {
	int ret = 0;
	int port = 9109;

	if (argc != 2) {
		printf("usage: cee_app.Linux.i686 mgmt_port\n");
		return WAVE_MESSAGE_SUCCESS;
	}

	port += atoi(argv[1]);

	/* Set DCM client hander  */
	setQOSMsgHandlerForClient();

	//CEE CONFIG/OPERATIONAL DATA MESSAGE HANDLERS BEGIN

	add_client_message_handler(QOSCLIENTCREATECEEMAP, printCeeMap);
	add_client_message_handler(QOSCLIENTCREATEPRIORITYGROUP,
			printPriorityGroup);
	add_client_message_handler(QOSCLIENTCREATEPRIORITYMAP, printPriorityMap);
	add_client_message_handler(QOSCLIENTCREATECEEREMAP, printCeeRemap);
	add_client_message_handler(QOSCLIENTDELETEPRIORITYGROUP,
			printPriorityGroup);
	add_client_message_handler(QOSCLIENTDELETEPRIORITYMAP, dummy);

	//CEE CONFIG/OPERATIONAL DATA MESSAGE HANDLERS END

	//QOS CONFIG/OPERATIONAL DATA MESSAGE HANDLERS BEGIN
	add_client_message_handler(QOSCLIENTCREATECOSTOCOSMUTATIONMAP,
			createCosToCosMutationMap);
	add_client_message_handler(QOSCLIENTDELETECOSTOCOSMUTATIONMAP,
			deleteCosToCosMutationMap);
	add_client_message_handler(QOSCLIENTCREATECOSTOTRAFFICCLASSMAP,
			createCosToTrafficClassMap);
	add_client_message_handler(QOSCLIENTDELETECOSTOTRAFFICCLASSMAP,
			deleteCosToTrafficClassMap);
	add_client_message_handler(QOSCLIENTCREATEQOSRCVQUEUEMULTICASTSETTING,
			createMcastSetting);
	add_client_message_handler(QOSCLIENTDELETEQOSRCVQUEUEMULTICASTSETTING,
			deleteMcastSetting);
	add_client_message_handler(QOSCLIENTCREATEQOSQUEUEMULTICASTSCHEDULER,
			createMcastScheduler);
	add_client_message_handler(QOSCLIENTDELETEQOSQUEUEMULTICASTSCHEDULER,
			deleteMcastScheduler);
	add_client_message_handler(QOSCLIENTCREATEQOSQUEUESCHEDULER,
			createScheduler);
	add_client_message_handler(QOSCLIENTDELETEQOSQUEUESCHEDULER,
			deleteScheduler);
	add_client_message_handler(QOSSHOWMESSAGE, dummy);
	add_client_message_handler(QOSCLIENTCONFIGINTFNSMATTRIBUTES,
			qosClientConfigIntf);

	add_client_message_handler(QOSCLIENTCLEAR, qosClearCounters);
	add_client_message_handler(QOSCLIENTCREATEDSCPTODSCPMUTATIONMAP,
			qosCreateDscpMutationMap);
	add_client_message_handler(QOSCLIENTDELETEDSCPTODSCPMUTATIONMAP,
			qosDeleteDscpMutationMap);
	add_client_message_handler(QOSCLIENTCREATEDSCPTOTRAFFICCLASSMAP,
			qosCreateDscpTrafficClassMap);
	add_client_message_handler(QOSCLIENTDELETEDSCPTOTRAFFICCLASSMAP,
			qosDeleteDscpTrafficClassMap);
	add_client_message_handler(QOSCLIENTCREATEDSCPTOCOSMAP,
			qosCreateDscpCosMap);
	add_client_message_handler(QOSCLIENTDELETEDSCPTOCOSMAP,
			qosDeleteDscpCosMap);
	add_client_message_handler(QOSCLIENTCREATEREDPROFILE, qosCreateRedProfile);
	add_client_message_handler(QOSCLIENTDELETEREDPROFILE, qosDeleteRedProfile);
	add_client_message_handler(QOSCLIENTCREATEDSCPMARKLIST,
			qosCreateDscpMarkList);
	add_client_message_handler(QOSCLIENTDELETECEEMAP, qosCreateCeeReMap);

	//QOS CONFIG/OPERATIONAL DATA MESSAGE HANDLERS END

	//FlowBased  QoS CONFIG/OPERATIONAL DATA MESSAGE HANDLERS BEGIN

	add_client_message_handler(POLICERDEBUG, PolicerDebugMessageHandler);

	add_client_message_handler(POLICERCLIENTPRIORITYMAP,
			PolicerPriorityMapMessageHandler);

	add_client_message_handler(POLICERCLIENTCLASSMAP,
			PolicerClassMapMessageHandler);

	add_client_message_handler(POLICERCLIENTPOLICYCLASSMAP,
			PolicerPolicyClassMapMessageHandler);

	add_client_message_handler(POLICERCLIENTTE, PolicerTeMessageHandler);

	add_client_message_handler(POLICER_SHOW_ALL, PolicerShowMessageHandler);

	add_client_message_handler(POLICER_CLEAR_ALL, PolicerClearMessageHandler);
	add_client_message_handler(QOSCLIENTNASCONFIG, QosClientNasHandler);

	//FlowBased  QoS CONFIG/OPERATIONAL DATA MESSAGE HANDLERS END

	WaveNs
	::FrameworkToolKit::setManagementInterfaceReceiverPort(port);

	/* Initialize Dcmd */
	ret = dcm_initialize_client("qos");
	if (ret != 0) {
		printf(
				"Failed :: Dcm-client Initialization failed from cee client... \n");
		assert(ret==0);
	} else {
		printf(
				"Passed :: Dcm-Client Initialization passed from cee client... \n");
	}

	while (1)
		sleep(1000000);

	return ret;

}
