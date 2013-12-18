/* **
 * udld_app.c
 *
 * June 15, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "uapp.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Udld/udld_dcm_def.h"
#include "ClientInterface/Udld/UdldDcmClientHandler.h"


#define COMPARENLEN(A,B)  strncmp(A, B, strlen(A))

#define GET_ATTRIBUTE_VALUE(_orig_str, _attribute) \
do { \
        char* _str = (char*) malloc(64*sizeof(char)); \
        strcpy(_str, _orig_str); \
        char _delims[] = "="; \
        _attribute = NULL; \
        _attribute = strtok(_str, _delims); \
        _attribute = strtok(NULL, _delims); \
    } while(0) \



int handle_udld_intf_conf (const void *inp, void ** const out)
{
    udld_mgmt_conf_req_t *req = (udld_mgmt_conf_req_t *) inp;

    printf("----- handle_udld_intf_conf ( udld_mgmt_conf_req_t *req ) -----\n");
    printf(" req->cmd_code                  = %u\n", req->cmd_code);
    printf(" req->entry.intf_conf.if_type   = %lu\n", req->entry.intf_conf.if_type);
    printf(" req->entry.intf_conf.if_name   = %s\n", req->entry.intf_conf.if_name);
    printf(" req->entry.intf_conf.disable   = %u\n", req->entry.intf_conf.disable);

    printf("\n\n");
    fflush(stdout);
    return 0;
}


int
udld_dcm_clear_handler(const void *input_p, void* output_pp) {
udld_mgmt_req_t *req_p = (udld_mgmt_req_t*) input_p;
udld_mgmt_clear_resp_t *resp_p = NULL;

int returnCode = 0;

if (req_p == NULL) {
	returnCode = 0;
	goto returnFromMethod;
}

printf("udld_dcm_clear_handler: Enter");
printf("udld_dcm_clear_handler: CommandCode<%d>", req_p->cmd_code);
printf("udld_dcm_clear_handler: InterfaceType<%s>",
		req_p->entry.intf.interface);
printf("udld_dcm_clear_handler: InterfaceIndex<%s>",
		req_p->entry.intf.if_name);

resp_p = (udld_mgmt_clear_resp_t *) calloc(1, sizeof(udld_mgmt_clear_resp_t));

if (resp_p == NULL) {
	returnCode = 0;
	goto returnFromMethod;
}

resp_p->ret_code = UDLD_MGMT_SUCCESS;

switch (req_p->cmd_code) {
	case UDLD_MGMT_CMD_CLEAR_STATS:
	printf("cmd_code:%x", req_p->cmd_code);
	resp_p->ret_code = UDLD_MGMT_SUCCESS;
	printf("udld_dcm_clear_handler: rc before mapping11 <%d>",resp_p->ret_code);
	printf("udld_dcm_clear_handler: rc before mapping12 <%d>",resp_p->ret_code);
	returnCode = resp_p->ret_code;
	printf("udld_dcm_clear_handler: rc before mapping13 <%d>",resp_p->ret_code);
	output_pp = (void*)resp_p;
	break;

	case UDLD_MGMT_CMD_CLEAR_STATS_INTF:
	case UDLD_MGMT_CMD_CLEAR_STATS_INTF_GI:
	case UDLD_MGMT_CMD_CLEAR_STATS_INTF_FO:
	printf("cmd_code:%x intf:%s", req_p->cmd_code, req_p->entry.intf.if_name);
	resp_p->ret_code = UDLD_MGMT_SUCCESS;
	printf("udld_dcm_clear_handler: rc before mapping <%d>",resp_p->ret_code);
	returnCode = resp_p->ret_code;
	output_pp = (void*)resp_p;
	break;

	case UDLD_MGMT_CMD_UNKNOWN:
	case UDLD_MGMT_CMD_SHOW_GBL:
	case UDLD_MGMT_CMD_SHOW_INTF:
	case UDLD_MGMT_CMD_SHOW_INTF_GI:
	case UDLD_MGMT_CMD_SHOW_INTF_FO:
	case UDLD_MGMT_CMD_SHOW_STATS:
	case UDLD_MGMT_CMD_SHOW_STATS_INTF:
	case UDLD_MGMT_CMD_SHOW_STATS_INTF_GI:
	case UDLD_MGMT_CMD_SHOW_STATS_INTF_FO:
	default:
	printf("udld_dcm_clear_handler: Unexpected command code %d",req_p->cmd_code);
	break;
}

printf("udld_dcm_clear_handler: rc before mapping <%d>",returnCode);
if(NULL == output_pp) {
	printf("udld_dcm_clear_handler: output_pp is NULL");
} else {
	printf("udld_dcm_clear_handler: output_pp is not NULL");
}


returnFromMethod:
return ((returnCode));
}

int udldOperationalDataHandler(const void *inp, void ** const out) {

	printf("udldOperationalDataHandler: Entering");

	//int returnCode = 0;
	const char* interfaceType = "";
	const char* interfaceIndex = "";
	const char* flowDirection = "";
	int iterator = 0;
	void* outBuffer = NULL;

	UnifiedClientMessageInput_t* message = (UnifiedClientMessageInput_t*) inp;
	char* arr = (char*) message->inputBuffer;

	printf("input=%s\n", arr);

	switch (message->groupCode) {

	case 3001: {

		printf("udldOperationalDataHandler -- clear udld statistics \n");
		udld_mgmt_req_t* clearUDLDStats = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));
		clearUDLDStats->cmd_code = UDLD_MGMT_CMD_CLEAR_STATS;
		inp = (void*) clearUDLDStats;

		outBuffer = (void*) calloc(1, sizeof(udld_mgmt_clear_resp_t));
		udld_dcm_clear_handler(inp, outBuffer);

		if (NULL == outBuffer) {
			printf(
					"udldOperationalDataHandler -- clear udld statistics outBuffer is NULL \n");

		} else {
			udld_mgmt_clear_resp_t* some = (udld_mgmt_clear_resp_t*) outBuffer;
			printf("some : <%d>", some->ret_code);
		}

		break;
	}

	case 3002: {
		printf(
				"udldOperationalDataHandler -- clear udld statistics interface interface-type interface-index \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {

			printf(
					"udldOperationalDataHandler -- clear udld statistics interface interface-type interface-index <%s> \n",
					message->commandString[iterator]);

			if (COMPARENLEN("interface-index",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceIndex);
			} else if (COMPARENLEN("interface-type",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceType);
			}
		}
		printf(
				"udldOperationalDataHandler -- clear udld statistics interface interface-type : <%s> interface-index : <%s> \n",
				interfaceType, interfaceIndex);

		udld_mgmt_req_t* clearUDLDStats = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));
		clearUDLDStats->cmd_code = UDLD_MGMT_CMD_CLEAR_STATS_INTF;

		strncpy(clearUDLDStats->entry.intf.if_name, interfaceIndex,
				(UDLD_IF_STR_MAX_LEN - 1));
		clearUDLDStats->entry.intf.if_name[UDLD_IF_STR_MAX_LEN] = '\0';

		strncpy(clearUDLDStats->entry.intf.interface, interfaceType,
				(UDLD_IF_STR_MAX_LEN - 1));
		clearUDLDStats->entry.intf.interface[UDLD_IF_STR_MAX_LEN] = '\0';

		inp = (void*) clearUDLDStats;

		//returnCode = udld_dcm_clear_handler(inp, out);
		break;
	}
	case 3003: {

		printf("udldOperationalDataHandler -- show udld \n");

		udld_mgmt_req_t* showUDLDBuffers = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));
		showUDLDBuffers->cmd_code = UDLD_MGMT_CMD_SHOW_GBL;
		inp = (void*) showUDLDBuffers;

		//returnCode = udld_dcm_show_handler(inp, out);
		break;
	}
	case 3004: {
		printf(
				"udldOperationalDataHandler -- show udld interface interface-type interface-index \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("interface-type", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceType);
			} else if (COMPARENLEN("interface-index",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceIndex);
			}
		}

		printf(
				"udldOperationalDataHandler -- show udld interface interface-type : <%s> interface-index : <%s> \n",
				interfaceType, interfaceIndex);

		udld_mgmt_req_t* showUDLDBuffers = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));

		showUDLDBuffers->cmd_code = UDLD_MGMT_CMD_SHOW_INTF;

		strncpy(showUDLDBuffers->entry.intf.if_name, interfaceIndex,
				(UDLD_IF_STR_MAX_LEN - 1));
		showUDLDBuffers->entry.intf.if_name[UDLD_IF_STR_MAX_LEN] = '\0';

		strncpy(showUDLDBuffers->entry.intf.interface, interfaceType,
				(UDLD_IF_STR_MAX_LEN - 1));
		showUDLDBuffers->entry.intf.interface[UDLD_IF_STR_MAX_LEN] = '\0';

		inp = (void*) showUDLDBuffers;

		//returnCode = udld_dcm_show_handler(inp, out);
		break;
	}
	case 3005: {
		printf("udldOperationalDataHandler -- show udld statistics \n");

		udld_mgmt_req_t* showUDLDStats = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));
		showUDLDStats->cmd_code = UDLD_MGMT_CMD_SHOW_STATS;
		inp = (void*) showUDLDStats;

		//returnCode = udld_dcm_show_handler(inp, out);
		break;
	}
	case 3006: {
		printf(
				"udldOperationalDataHandler -- show udld statistics interface interface-type interface-index \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("interface-type", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceType);
			} else if (COMPARENLEN("interface-index",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceIndex);
			}
		}
		printf(
				"udldOperationalDataHandler -- show udld statistics interface interface-type : <%s> interface-index : <%s> \n",
				interfaceType, interfaceIndex);
		udld_mgmt_req_t* showUDLDStats = (udld_mgmt_req_t*) (malloc(
				sizeof(udld_mgmt_req_t)));
		showUDLDStats->cmd_code = UDLD_MGMT_CMD_SHOW_STATS;

		strncpy(showUDLDStats->entry.stats_intf.if_name, interfaceIndex,
				(UDLD_IF_STR_MAX_LEN - 1));
		showUDLDStats->entry.stats_intf.if_name[UDLD_IF_STR_MAX_LEN] = '\0';

		strncpy(showUDLDStats->entry.stats_intf.interface, interfaceType,
				(UDLD_IF_STR_MAX_LEN - 1));
		showUDLDStats->entry.stats_intf.interface[UDLD_IF_STR_MAX_LEN] = '\0';

		inp = (void*) showUDLDStats;

		//returnCode = udld_dcm_show_handler(inp, out);
		break;
	}
	case 3007: {
		printf(
				"udldOperationalDataHandler -- debug udld packet all flow-direction \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("flow-direction", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						flowDirection);
			}
		}
		printf(
				"udldOperationalDataHandler -- debug udld packet all flow-direction : <%s> \n",
				flowDirection);
		udld_debug_cmd_req_t* debugUDLDPackets =
				(udld_debug_cmd_req_t*) (malloc(sizeof(udld_debug_cmd_req_t)));

		debugUDLDPackets->cmd_code = UDLD_DEBUG_CMD_CODE_PACKET_ALL;
		debugUDLDPackets->debug_flags =
				(strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_BOTH, 4) == 0) ?
						UDLD_DEBUG_REQ_FLAG_TXRX :
						((strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_TX, 2) == 0) ?
								UDLD_DEBUG_REQ_FLAG_TX : UDLD_DEBUG_REQ_FLAG_RX);
		inp = (void*) debugUDLDPackets;

		//returnCode = udld_dcm_debug_handler(inp, out);
		break;
	}
	case 3008: {
		printf(
				"udldOperationalDataHandler -- debug udld packet interface interface-type interface-index flow-direction \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("interface-type", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceType);
			} else if (COMPARENLEN("interface-index",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceIndex);
			} else if (COMPARENLEN("flow-direction",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						flowDirection);
			}
		}

		printf(
				"udldOperationalDataHandler -- debug udld packet interface interface-type : <%s> interface-index : <%s> flow-direction : <%s> \n",
				interfaceType, interfaceIndex, flowDirection);

		udld_debug_cmd_req_t* debugUDLDPackets =
				(udld_debug_cmd_req_t*) (malloc(sizeof(udld_debug_cmd_req_t)));
		debugUDLDPackets->cmd_code = UDLD_DEBUG_CMD_CODE_PACKET_INTF;
		debugUDLDPackets->debug_flags =
				(strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_BOTH, 4) == 0) ?
						UDLD_DEBUG_REQ_FLAG_TXRX :
						((strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_TX, 2) == 0) ?
								UDLD_DEBUG_REQ_FLAG_TX : UDLD_DEBUG_REQ_FLAG_RX);

		strncpy(debugUDLDPackets->intf.if_name, interfaceIndex,
				(UDLD_IF_STR_MAX_LEN - 1));
		debugUDLDPackets->intf.if_name[UDLD_IF_STR_MAX_LEN] = '\0';

		strncpy(debugUDLDPackets->intf.interface, interfaceType,
				(UDLD_IF_STR_MAX_LEN - 1));
		debugUDLDPackets->intf.interface[UDLD_IF_STR_MAX_LEN] = '\0';

		inp = (void*) debugUDLDPackets;

		//returnCode = udld_dcm_debug_handler(inp, out);
		break;
	}
	case 3009: {
		printf(
				"udldOperationalDataHandler -- no debug udld packet all flow-direction \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("flow-direction", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						flowDirection);
			}
		}
		printf(
				"udldOperationalDataHandler -- no debug udld packet all flow-direction : <%s> \n",
				flowDirection);
		udld_debug_cmd_req_t* debugUDLDPackets =
				(udld_debug_cmd_req_t*) (malloc(sizeof(udld_debug_cmd_req_t)));

		debugUDLDPackets->cmd_code = UDLD_NO_DEBUG_CMD_CODE_PACKET_ALL;
		debugUDLDPackets->debug_flags =
				(strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_BOTH, 4) == 0) ?
						UDLD_DEBUG_REQ_FLAG_TXRX :
						((strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_TX, 2) == 0) ?
								UDLD_DEBUG_REQ_FLAG_TX : UDLD_DEBUG_REQ_FLAG_RX);
		inp = (void*) debugUDLDPackets;

		//returnCode = udld_dcm_debug_handler(inp, out);
		break;
	}
	case 3010: {
		printf(
				"udldOperationalDataHandler -- no debug udld packet interface interface-type interface-index flow-direction \n");

		for (iterator = 0; iterator < message->commandStringSize; iterator++) {
			if (COMPARENLEN("interface-type", message->commandString[iterator])
					== 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceType);
			} else if (COMPARENLEN("interface-index",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						interfaceIndex);
			} else if (COMPARENLEN("flow-direction",
					message->commandString[iterator]) == 0) {
				GET_ATTRIBUTE_VALUE(message->commandString[iterator],
						flowDirection);
			}
		}

		printf(
				"udldOperationalDataHandler -- no debug udld packet interface interface-type : <%s> interface-index : <%s> flow-direction : <%s> \n",
				interfaceType, interfaceIndex, flowDirection);

		udld_debug_cmd_req_t* debugUDLDPackets =
				(udld_debug_cmd_req_t*) (malloc(sizeof(udld_debug_cmd_req_t)));
		debugUDLDPackets->cmd_code = UDLD_NO_DEBUG_CMD_CODE_PACKET_INTF;
		debugUDLDPackets->debug_flags =
				(strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_BOTH, 4) == 0) ?
						UDLD_DEBUG_REQ_FLAG_TXRX :
						((strncasecmp(flowDirection, UDLD_DEBUG_CLI_TKN_STR_TX, 2) == 0) ?
								UDLD_DEBUG_REQ_FLAG_TX : UDLD_DEBUG_REQ_FLAG_RX);

		strncpy(debugUDLDPackets->intf.if_name, interfaceIndex,
				(UDLD_IF_STR_MAX_LEN - 1));
		debugUDLDPackets->intf.if_name[UDLD_IF_STR_MAX_LEN] = '\0';

		strncpy(debugUDLDPackets->intf.interface, interfaceType,
				(UDLD_IF_STR_MAX_LEN - 1));
		debugUDLDPackets->intf.interface[UDLD_IF_STR_MAX_LEN] = '\0';

		inp = (void*) debugUDLDPackets;

		//returnCode = udld_dcm_debug_handler(inp, out);
		break;
	}

	case 3011: {
		printf("udldOperationalDataHandler -- show debug udld \n");

		udld_debug_cmd_req_t* debugUDLDPackets =
				(udld_debug_cmd_req_t*) (malloc(sizeof(udld_debug_cmd_req_t)));

		debugUDLDPackets->cmd_code = UDLD_DEBUG_CMD_CODE_SHOW_STATUS;
		inp = (void*) debugUDLDPackets;

		//returnCode = udld_dcm_debug_handler(inp, out);
		break;
	}

	default: {
		printf("udldOperationalDataHandler -- Invalid cmdcode \n");
		break;
	}
	}

	UnifiedClientMessageOutput_t *result =
			(UnifiedClientMessageOutput_t *) malloc(
					sizeof(UnifiedClientMessageOutput_t));
	result->moreRecords = 0;

	char *output = NULL;

	output = (char *) malloc(20);
	strcpy(output, "hello world\n");
	output[12] = '\0';

	result->outputBufferSize = 20;
	result->outputBuffer = output;
	result->bufferTag = 1;

	(*out) = result;
	return (0);
}



int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("usage: %s <mgmt_port_offset>\n", argv[0]);
        return -1;
    }

    int mgmt_port_offset = atoi(argv[1]);

    setUdldDcmClientMsgHandler();

    add_client_message_handler(MSG_UDLD_PHY_INTF_CONF, handle_udld_intf_conf);
    add_client_message_handler(UNIFIEDCLIENTGETDATA, udldOperationalDataHandler);

    start_client(UDLD_DCM_CLIENT_NAME, mgmt_port_offset);

    return 0;
}


