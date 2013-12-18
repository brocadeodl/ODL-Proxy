#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <search.h>
#include <pthread.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ManagementInterface/ClientInterface/UnifiedClientCommon.h"
#include "PhysicalInterface/ClientInterface/PhysicalInterfaceManagementInterfaceMessagesHandler.h"
#include "ClientInterface/Nsm/NsmMangIntfHandler.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/PVlanCommon.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/APPM/APPMMessageDef.h"

#include "ClientInterface/Nsm/VlanClassifierCommon.h"
#include <assert.h>

#include "Fcoe/Common/FcoePluginTypes.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

#include "WaveCStatus.h"
#include "DcmCStatus.h"
#include "VcsFabric/Local/VcsNodeHaClientMsgCStruct.h"
#include "ClientInterface/InterfaceCommon.h"
#include "ClientInterface/Span/SpanMessageDef.h"
#include "uapp.h"
//#include "ClientDataInterface/GetDataFromClientSource.h"
#include "ClientInterface/tlv/tlv_dcm.h"

#define HAL_IF_CAP_EDGE     0x01
#define HAL_IF_CAP_ISL      0x02

#define SHUTDOWN 1
#define NO_SHUTDOWN 0
/*
int executeNSMCmd(int cmdCode, int subcmd, int argc, char ** argv,char *index);

int executeNSMCmd(int cmdCode, int subcmd, int argc, char ** argv,char *index)
{
    int i;
    printf("========== \n");
    printf("Cmd %d  subcmd : %d argc : %d \n",cmdCode, subcmd, argc);
   
    if(index != NULL)
        printf("%s \n",index );
    for(i=0;i<argc;i++)
        printf("\t[%d] %s \n",i ,argv[i] );
    printf("========== \n");
       
    return 1;
}
        
*/
/*VlanClassifier ProtoTypes*/
int printVlanClassifierRulecreate(const void *inp, void ** const out);
int printVlanClassifierRuledelete(const void *inp, void ** const out);
int printVlanClassifierGroupCreate(const void *inp, void ** const out);
int printVlanClassifierGroupdelete(const void *inp, void ** const out);
int printVlanClassifierActivateGroup(const void *inp, void ** const out);

int printUnifiedClientUpdate(const void *inp, void ** const out);

int printVlanIntf(const void *inp, void ** const out);
int printPoIntf(const void *inp, void ** const out);
int printPhyIntf(const void *inp, void ** const out);
int printShowIntf(const void *inp, void ** const out);
int printShowSfp(const void *inp, void ** const out);
int printVlanClassifierRule(const void *inp, void ** const out);
int printFcoeportAttr(const void *inp, void ** const out);
int printFcoeVlan(const void *inp, void ** const out);
int printReservedVlan(const void *inp, void ** const out);
int nsmIpConfigHandler(const void *inp, void ** const out);
int defaultIntfConfHandler(const void *inp, void ** const out);
int nsm_vrf_config_handler(const void *inp, void ** const out);

int vlan_provision_handler(const void *inp, void ** const out);
int pvlan_assoc_handler(const void *inp, void ** const out);
int pvlan_mapping_handler(const void *inp, void ** const out);
int macGroupConfigMessageHandler (const void *inp, void ** const out);


//AMPP Message Handlers.
int portPHandler(const void *inp, void ** const out);
int portProfileDomainHandler(const void* inp, void** const out);
int vlanPHanlder( const void *inp, void ** const out);
int fcoePHandler( const void *inp, void ** const out );
int amppRestrictFloodingCommandHandler( const void *inp, void ** const out ); 
int amppShowMessageHandler(const void* inp, void** const out);
int amppPortProfileOrDomainToInterfaceAssociationHandler(const void* inp, void** const out);
int populateUnifiedClientGetData (const void *inp, void ** const out);
int configReplayVlanClassification(const void *inp, void ** const out);
int gVlanEnableHandler(const void *inp, void ** const out);
void *nsm_dcm_call_handler(void *);


int errorEnable = 0;

bool g_cpActive = false;

void *provisioned_vlans = NULL, *range_vlans = NULL;

int span_config_handler(const void *inp, void ** const out)
{
   struct span_config_msg_t *msg = (span_config_msg_t *)inp;
   struct span_config_error_t *nsm_msg = (struct span_config_error_t *)calloc(1,sizeof(span_config_error_t));

   nsm_msg->error_result = NULL;
   nsm_msg->error_result =(int *)calloc(1,sizeof(int));
   *(nsm_msg->error_result) =25;

   if(msg == NULL) {
	   return -1;	
   }

    switch(msg->m_messageType) {

	case SPAN_LOCAL_MSG_DESC:
   		*out = NULL;
		break;	

	case SPAN_LOCAL_MSG_CONFIG:
			// state 0,1 stand for disable and enabled, 2 stand for error propagation.
			nsm_msg->m_state = 2; 
			*out = (void *)nsm_msg;
			break;
	case SPAN_LOCAL_MSG_SESSION:
    	*out = NULL;
		break;
	default:
        break;
	}

    return 0;
}

int cpStateChangeHandler (const void *inp, void ** const out)
{
	CpState_InputMsg_t *cpStateChangeMsg = (CpState_InputMsg_t *) inp;

	uint32_t cpState = cpStateChangeMsg->cpState;
	printf ("nsm_app::cpStateChangeHandler cpState = %d\n",cpState);
	g_cpActive = true;
	return (0);
}

int getIntfAll(void **out)
{
    uint32_t  num_records = 0;
    uint32_t index = 0;
    char buffer[10];

    nsm_intf_all_msg_t *payload_p = NULL;
    nsm_intf_msg_t *ptr;

    num_records = 5;

    if (num_records > 0) {
        payload_p = (nsm_intf_all_msg_t *) calloc(1, sizeof(nsm_intf_all_msg_t) + (num_records - 1)*sizeof (nsm_intf_msg_t));
        /* populate header */
        payload_p->num_records = num_records;
        payload_p->record_type = NSM_DEFAULT_CONFIG_RESPONSE;

        for(index =0 ; index <5; index++)
        {

            ptr = (nsm_intf_msg_t *)&(payload_p->intfdetails[index]);
            sprintf(buffer,"te0/%d",index);

            strcpy(ptr->ifName, buffer);
            strcpy(ptr->desc, buffer);
            ptr->m_ifIndex = index;
            ptr->islState = EDGE;
            ptr->status = SHUTDOWN;
            ptr->mtu = 1500 + index;
            
            printf("buffer ..... %s %s \n",buffer,ptr->ifName);
        }
    } else {
        payload_p = (nsm_intf_all_msg_t  *) calloc(1, sizeof(lacp_show_msg_t));
        payload_p->num_records = 0;
        payload_p->record_type = NSM_DEFAULT_CONFIG_RESPONSE;
        printf("Else ... \n");
    }

    *out = payload_p;

    {
        nsm_intf_all_msg_t *msg = ((nsm_intf_all_msg_t *)*out);
        printf("No of Records %d \n", msg->num_records);
        printf("Record Type %d \n", msg->record_type);
    }
    return 1;
}
int nsm_vrf_config_handler(const void *inp, void ** const out)
{
    char if_name[65];
    nsm_vrf_global_config_msg_t *msg = (nsm_vrf_global_config_msg_t *)inp;

    if (msg == NULL)
        return (-1);
    GENERATE_SHORT_IF_NAME_FOR_BACK_END(msg->if_type, msg->if_name, if_name, 64);
    printf ("Opcode: %d - ifname:%s - ifType:%d - vrfName:%s shortname:%s\n",
                    msg->opcode, msg->if_name, msg->if_type, msg->vrf_name, if_name);
    fflush(stdout);

    *out = NULL;
    return (0);


    
}

int defaultIntfConfHandler (const void *inp, void ** const out)
{
    nsm_req_msg_t *msg = (nsm_req_msg_t *)inp;
    int ret = 0;

    if (msg == NULL)
        return (-1);

    printf("Geting interface details... \n");
    switch(msg->cmdCode)
    {
        case DEFAULT_INTERFACE:
            printf("Calling api to get interface details... \n");
            ret = getIntfAll(out);
            break;
        default:
            break;
    }

    if(ret < 0 )
        return -1;

    return (0);
}

int printGlobalConfig (const void *inp, void ** const out)
{
    NsmGlbUpdConfMsg_t *msg = (NsmGlbUpdConfMsg_t *)inp;
    int ret = 0;

    if (msg == NULL)
        return (-1);

    printf("Global Configs... \n");
    switch(msg->cmdCode)
    {
        case NSMUPDATE_NATIVE_VLAN:
            printf("NativeVlan: %d \n",msg->nativevlanDisable);
            break;
        default:
            break;
    }

    if(ret < 0 )
        return -1;

    return (0);
}

int printFcoeVlan(const void *inp, void ** const out)
{
    NsmFcoeVlanMsg_t *msg_p = (NsmFcoeVlanMsg_t *)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return 0;
    }

    printf("Oper=%d, OldVlan=%d, NewVlan=%d, Name=%s, Ifindex=%d, Desc=%s\n",
           msg_p->cmd_code,
           msg_p->old_vlan,
           msg_p->new_vlan,
           msg_p->vlanName,
           msg_p->vlanIfIndex,
           msg_p->desc);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printReservedVlan(const void *inp, void ** const out)
{
    NsmReservedVlanMsg_t *msg_p = (NsmReservedVlanMsg_t *)inp;

    if (msg_p == NULL) {
        printf ("msg is NULL");
        return -1;
    }

    printf("Oper=%d, start=%d, end=%d\n",
           msg_p->cmdcode,
           msg_p->start,
           msg_p->end);
    dcm_debug_printf("Oper=%d, start=%d, end=%d\n",
          msg_p->cmdcode,
          msg_p->start,
          msg_p->end);


    return 0;
}

int printFcoeportAttr(const void *inp, void ** const out)
{
    NsmFcoeportMsg_t *msg_p = (NsmFcoeportMsg_t *)inp;
    unsigned int i;
    if (msg_p == NULL) {
        printf ("msg is NULL");
        return 0;
    }

    printf("FCOE Port Command for INTF=%s\n", msg_p->if_name);
    printf("Cmd=%d ", msg_p->cmd_code);
    printf(" Vlans=");
    for (i = 0; i < msg_p->num_vlans; i++) {
        printf("%d ", msg_p->fcoe_vlan_list[i]);
    }
    printf("  Cee-Map=%s", msg_p->cee_map_name);
    printf(" Fcoe-Map=%s\n", msg_p->map_name);
    fflush(stdout);

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int printVlanClassifierRule(const void *inp, void ** const out)
{
    VlanClassifierCreateRuleMsg_t  *msg = (VlanClassifierCreateRuleMsg_t *)inp;

    if (msg == NULL) 
    {
	printf("------>msg is Null\n");
        return (-1);
    } 

        printf("ruleId : %d\n", msg->ruleId);
        printf("protoval : %d\n", msg->protoVal);
        printf("encap : %d\n", msg->encap);
        printf("mac : %s \n", msg->mac);

    *out = NULL;

    return (0);

}

int printUnifiedClientUpdate(const void *inp, void ** const out)
{
    printf("Unified Client Update message handled.");

    UnifiedClientMessage_t *msg = (UnifiedClientMessage_t *) inp;

    printf("Unified Client Update message handled. attribute size = %d \n", msg->attributeSize);

    return (0);
}

typedef struct range_vlans_ {
    void *range;
    int count;
} range_vlans_t;

void *xmalloc(unsigned n) {
    void *p;
    p = malloc(n);
    if (p == NULL)
        fprintf(stderr, "insufficient memory\n");
    return p;
}

int compare(const void *pa, const void *pb) {
    if (*(uint32_t *) pa < *(uint32_t *) pb)
        return -1;
    if (*(uint32_t *) pa > *(uint32_t *) pb)
        return 1;
    return 0;
}

void *insertIntoTree(void **rootP, uint32_t value) {
    void *val;
    uint32_t *ptr = (uint32_t *) xmalloc(sizeof(uint32_t));
    *ptr = value;
    val = tsearch( ptr, rootP, compare);
    return val;
}

void insert(uint32_t value, range_vlans_t *rangeVlans) {
    void * pValue;
    vlan_provision_status_record_t *vlanNode = (vlan_provision_status_record_t *) malloc(
            sizeof(vlan_provision_status_record_t));
    vlanNode->vlan_id = value;

    /*printf("finding vlan in provisioned_vlans %lu\n",
            ( unsigned long )vlanNode->vlan_id );*/
    pValue = tfind(&value, &provisioned_vlans, compare);

    if (pValue) { // already provisioned
        vlanNode->provision_status = NSM_VLAN_ALREADY_PROVISIONED;
    } else {
        //printf("Did not find %lu\n", ( unsigned long )value);
        vlanNode->provision_status = NSM_VLAN_PROVISIONED_NOW;
    }

    //printf("inserting into range tree %lu\n", vlanNode->vlan_id );
    pValue = tsearch( vlanNode, &rangeVlans->range, compare);
    if (pValue == NULL) {
        fprintf(stderr, "Could not add it to range tree\n");
    }
    rangeVlans->count++;

    if (vlanNode->provision_status == NSM_VLAN_PROVISIONED_NOW ) {
        pValue = insertIntoTree(&provisioned_vlans, value);
        if (pValue == NULL) {
            fprintf(stderr, "Could not add it to provisioned_vlans tree\n");
        }
    }
}

int response_index;
nsm_vlan_provision_response_t *response;

void fillResponse(const void *nodep, const VISIT which, const int depth) {
    uint32_t index;
    vlan_provision_status_record_t *vlanNode;
    switch (which) {
    case preorder:
        break;
    case postorder:
        index = response_index++;
        vlanNode = *(vlan_provision_status_record_t **) nodep;
        response->status[index].vlan_id = vlanNode->vlan_id;
        response->status[index].provision_status = vlanNode->provision_status;
        break;
    case endorder:
        break;
    case leaf:
        index = response_index++;
        vlanNode = *(vlan_provision_status_record_t **) nodep;
        response->status[index].vlan_id = vlanNode->vlan_id;
        response->status[index].provision_status = vlanNode->provision_status;
        break;
    }
}

void addRange(char *range) {

    const char dash = '-', c[2] = ",";
    char *token, *dR, *dL;
    size_t l;
    uint32_t lt, rt, min, max, value;

    range_vlans_t *rangeVlans;
    rangeVlans = (range_vlans_t*)calloc(1, sizeof(range_vlans_t));
    rangeVlans->count = 0;
    response_index = 0;

    /* get the first token */
    token = strtok(range, c);

    /* walk through other tokens */
    while (token != NULL) {
       // printf(" %s\n", token);
        dL = token;
        dR = strchr(token, dash);
        if (dR) {
            l = strlen(token) - strlen(dR);
            dL = strndup(token, l);
            dR++;
           // printf(" dL=%s\tdR=%s\n", dL, dR);
            lt = atol(dL);
            rt = atol(dR);
           // printf(" lt=%d\trt=%d\n", lt, rt);
            min = lt < rt ? lt : rt;
            max = lt < rt ? rt : lt;
            for (value = min; value <= max; value++) {
                insert(value, rangeVlans);
            }
        } else {
            lt = atol(dL);
            insert(lt, rangeVlans);
        }
        token = strtok(NULL, c);
    }


    size_t size =  (sizeof(nsm_vlan_provision_response_t) +
            rangeVlans->count * sizeof(vlan_provision_status_record_t));


    response = (nsm_vlan_provision_response_t *)calloc(1, size);

    response->number_of_records = rangeVlans->count;

    twalk(rangeVlans->range, fillResponse);

    tdestroy(rangeVlans->range, free);
    rangeVlans->range = NULL;
    free(rangeVlans);

    /*for (i = 0; i < response->number_of_records; i++) {
        printf("VLAN=%d Provision = %d\n",
                response->status[i].vlan_id,
                response->status[i].provision_status);
    }*/

}
int pvlan_mapping_handler(const void *inp, void ** const out)
{
    PVlanMappingMsg_t *msg = (PVlanMappingMsg_t *)inp;
    const char *func = __FUNCTION__;
       printf("%s  OpCode:%d\n", func, msg->opcode);
       printf("%s  PortName:%s\n", func, msg->PortName);
       printf("%s  ifType:%d\n", func, msg->ifType);
       printf("%s  primaryVlanId:%d\n", func, msg->primaryVlanId);
       printf("%s  secVlanRange:%s\n", func, msg->secVlanRange);

       return 0;
}
int pvlan_assoc_handler(const void *inp, void ** const out)
{
    PVlanTrunkAssocMsg_t *msg = (PVlanTrunkAssocMsg_t *)inp;
    const char *func = __FUNCTION__;
    printf("%s  OpCode:%d\n", func, msg->opcode);
    printf("%s  PortName:%s\n", func, msg->PortName);
    printf("%s  ifType:%d\n", func, msg->ifType);
    printf("%s  primaryVlanId:%d\n", func, msg->primaryVlanId);
    printf("%s  secondaryVlanId:%d\n", func, msg->secondaryVlanId);

    return 0;
}
int vlan_provision_handler(const void *inp, void ** const out)
{
	nsm_vlan_provision_request_t *request = (nsm_vlan_provision_request_t *)inp;

	printf("vlanProvisionHandler provision_code :%d\n", request->provision_code);
    printf("vlanProvisionHandler vlanRange :%s\n", request->vlan_range);

    if (PROVISION_NOT_REQUIRED != request->provision_code) {
        addRange(request->vlan_range);

        tdestroy(range_vlans, free);
        range_vlans = NULL;
    } else {
        response = (nsm_vlan_provision_response_t *)calloc(1, sizeof(nsm_vlan_provision_response_t));
        response->number_of_records = 0;
    }
    *out = (void *)response;
	return 0;
}

int printVlanIntf(const void *inp, void ** const out)
{
    NsmVlanIntfMsg_t *msg = (NsmVlanIntfMsg_t *)inp;
    int status = 0;

    if (msg == NULL)
        return (-1);

        printf("NSMUPDATE_VLAN_BULK_CONF ::%d \n",NSMUPDATE_VLAN_BULK_CONF);
        printf("Vlan cmdCode :%d\n", msg->cmdCode);
        printf("\tId : %d\n", msg->vlanId);
        printf("\tName : %s\n", msg->vlanName);
        if(msg->cmdCode == NSMUPDATE_VLAN_BULK_CONF)
        {
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_CREATE_CMD)) {
                printf("\tAdd Vlan : %d \n", msg->vlanId);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_DESC_CMD)) {
                printf("\tDesc : %s \n", msg->desc);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_STATUS_CMD)) {
                printf("\tFlag : %d \n", msg->flag);
            }
        } else {
            switch( msg->cmdCode) {
                   case NSMCREATE_VLAN_INTF:
                        printf("\tIfIndex : %u\n", msg->vlanIfIndex);
                        printf("\tCreate vlan %d \n",msg->vlanId);
                        break;
                   case NSMDELETE_VLAN_INTF:
                        printf("\tDelete vlan %d \n",msg->vlanId);
                        break;
                   case NSMUPDATE_VLAN_DESC:
                   case NSMUPDATE_VLAN_DELDESC:
                        printf("\tDesc : %s \n", msg->desc);
                        break;
                   case NSMUPDATE_VLAN_STATUS:
                   case NSMUPDATE_VLAN_DELSTATUS:     
                        printf("\tFlag : %d\n", msg->flag);
                        break;
                   default:
                        printf("\tIfIndex : %u\n", msg->vlanIfIndex);
                        printf("\tDesc : %s \n", msg->desc);
                        printf("\tFlag : %d\n", msg->flag);
                        break;
            }
        }
        

    *out = NULL;

        if(errorEnable)
        {
            if(msg->bulkCmdCode == NSMUPDATE_VLAN_BULK_CONF)
            {
                if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_CREATE_CMD)) {
                    printf("\tAdd Vlan : %d \n", msg->vlanId);
                    status = 1;
                    
                }
                if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_DESC_CMD)) {
                    printf("\tDesc : %s \n", msg->desc);
                    status = 1;
                    
                }
                if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_VLAN_STATUS_CMD)) {
                    printf("\tFlag : %d \n", msg->flag);
                    status = 1;
                }
            } else {
                switch( msg->cmdCode) {
                    case NSMCREATE_VLAN_INTF:
                        printf("\tIfIndex : %u\n", msg->vlanIfIndex);
                        printf("Create vlan %d \n",msg->vlanId);
                        //status = 1;
                        break;
                    case NSMDELETE_VLAN_INTF:
                        printf("Delete vlan %d \n",msg->vlanId);
                        status = 1;
                        break;
                    case NSMUPDATE_VLAN_DESC:
                    case NSMUPDATE_VLAN_DELDESC:
                        printf("\tDesc : %s \n", msg->desc);
                        status = 1;
                        break;
                    case NSMUPDATE_VLAN_STATUS:
                    case NSMUPDATE_VLAN_DELSTATUS:
                        printf("\tFlag : %d\n", msg->flag);
                        status = 1;
                        break;
                    default:
                        printf("\tIfIndex : %u\n", msg->vlanIfIndex);
                        printf("\tDesc : %s \n", msg->desc);
                        printf("\tFlag : %d\n", msg->flag);
                        break;

                }
            }
            
        }
    
    printf("status : %d \n",status);  
    return (status);


}

int printPoIntf(const void *inp, void ** const out)
{
    NsmPoIntfMsg_t *msg = (NsmPoIntfMsg_t *)inp;
    int status = 0;

    if (msg == NULL)
        return (-1);

        printf("NSMUPDATE_PO_BULK_CONF ::%d \n",NSMUPDATE_PO_BULK_CONF);
        printf("Po cmdCode :%d\n", msg->cmdCode);
        printf("\tId : %s\n", msg->poId);
        printf("\tName : %s\n", msg->poName);
                
        if(msg->cmdCode == NSMUPDATE_PO_BULK_CONF)
        {
            printf("\t BulkCmdCode : %d \n", msg->bulkCmdCode);
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_CREATE_CMD)) {
                printf("\tAdd Po : %s \n", msg->poId);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_DESC_CMD)) {
                printf("\tDesc : %s \n", msg->desc);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_STATUS_CMD)) {
                printf("\tFlag : %d \n", msg->flag);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_IGNORE_SPLIT_CMD)) {
                printf("\tIgnore-Split : %d \n", msg->ignore_split);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_NO_IGNORE_SPLIT_CMD)) {
                printf("\t(no) Ignore-Split : %d \n", msg->ignore_split);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MTU_CMD)) {
                printf("\tMtu : %d \n", msg->mtu); 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MODETYPE_CMD)) {
                printf("\tchannelgroup  :%d  %d \n", msg->poMode, msg->poType);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_CMD)) {
                printf("\tSw \n");
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MODE_CMD)) {
                printf("\tsw Mode : %d \n", msg->swMode); 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_ADD_ACCESS_VLAN_CMD)) {
                printf("\tAdd Access : %s \n", msg->swVlanName);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_TRUNK_VLAN_ALL_CMD)) {
                printf("\ttrunk all  \n");
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_ADD_TRUNK_VLAN_CMD)) {
                printf("\tadd trunk vlan : %s \n", msg->swVlanName);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD)) {
                printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN_CMD)) {
                printf("\texcept trunk vlan : %s \n", msg->swVlanName);
            }

        } else {

            switch(msg->cmdCode){
                case NSMCREATE_PO_INTF:
                    printf("\tCreate Po : %s \n", msg->poId);
                    break;
                case NSMDELETE_PO_INTF:
                    printf("\tDelete Po : %s \n", msg->poId);
                    break;
                case NSMUPDATE_PO_DESC:
                case NSMUPDATE_PO_DELDESC:
                    printf("\tDesc : %s \n", msg->desc);
                    break;
                case NSMUPDATE_PO_STATUS:
                case NSMUPDATE_PO_DELSTATUS:
                    printf("\tFlag : %d \n", msg->flag);
                    break;
                case NSMUPDATE_PO_IGNORE_SPLIT:
                case NSMUPDATE_PO_NO_IGNORE_SPLIT:
                    printf("\tIgnore-Split : %d \n", msg->ignore_split);
                    break;
                case NSMUPDATE_PO_MTU:
                case NSMUPDATE_PO_DELMTU:
                    printf("\tMtu : %d \n", msg->mtu); 
                    break;
                case NSMUPDATE_PO_SW:
                case NSMUPDATE_PO_DELSW:
                    printf("\tSw %d \n",msg->swModeFlag);
                    break;
                case NSMUPDATE_PO_MODE:
                case NSMUPDATE_PO_DELMODE:
                    printf("\tsw Mode : %d \n", msg->swMode); 
                    break;
                case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
                case NSMUPDATE_PO_SW_DELETE_ACCESS_VLAN:
                    printf("\tAdd Access : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
                case NSMUPDATE_PO_SW_TRUNK_VLAN_NONE:
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    break;
                case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:    
                    printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
                    printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:    
                    printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PO_SET_MAC_ADDRESS:
                case NSMUPDATE_PO_DEL_MAC_ADDRESS:
					printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->mac[0], msg->mac[1], msg->mac[2], msg->mac[3], msg->mac[4], msg->mac[5]);
                    break;
                case NSMUPDATE_PO_SET_MAC_GROUP:
                case NSMUPDATE_PO_DEL_MAC_GROUP:
					printf("macGroupId:%d.\n", msg->macGroupId);
                    break;
                case NSMUPDATE_PO_SET_CTAG:
                case NSMUPDATE_PO_DEL_CTAG:
                case NSMUPDATE_PO_PVLAN_SET_CTAG:
                case NSMUPDATE_PO_PVLAN_DEL_CTAG:
                    printf("vlanId:%s.\n", msg->swVlanName);
					printf("macGroupId:%d.\n", msg->ctagId);
                    break;
                default:
                        printf("\tDesc : %s \n", msg->desc);
                        printf("\tShutdown Flag : %d\n", msg->flag);
                        printf("\tIfIndex : %u\n", msg->poIfIndex);

                        printf("\tSwModeFlag : %d\n", msg->swModeFlag);
                        printf("\tSwMode : %d\n", msg->swMode);
                        printf("\tMtu : %d\n", msg->mtu);
                        printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                        printf("\tNative vlan : %d \n", msg->native_vlan_id);
        }
        }

    *out = NULL;

    if(errorEnable)
    {
        if(msg->bulkCmdCode == NSMUPDATE_PO_BULK_CONF)
        {
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_CREATE_CMD)) {
                printf("\tAdd Po : %s \n", msg->poId);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_DESC_CMD)) {
                printf("\tDesc : %s \n", msg->desc);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_STATUS_CMD)) {
                printf("\tFlag : %d \n", msg->flag);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_IGNORE_SPLIT_CMD)) {
                printf("\tIgnore-Split : %d \n", msg->ignore_split);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MTU_CMD)) {
                printf("\tMtu : %d \n", msg->mtu); 
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MODETYPE_CMD)) {
                printf("\tchannelgroup  :%d  %d \n", msg->poMode, msg->poType);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_CMD)) {
                printf("\tSw \n");
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_MODE_CMD)) {
                printf("\tsw Mode : %d \n", msg->swMode); 
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_ADD_ACCESS_VLAN_CMD)) {
                printf("\tAdd Access : %s \n", msg->swVlanName);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_TRUNK_VLAN_ALL_CMD)) {
                printf("\ttrunk all  \n");
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_ADD_TRUNK_VLAN_CMD)) {
                printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN_CMD)) {
                printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                status = 1; 
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN_CMD)) {
                printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                status = 1; 
            }

        } else {

            switch(msg->cmdCode){
                case NSMCREATE_PO_INTF:
                    printf("\tAdd Po : %s \n", msg->poId);
    //              status = 1; 
                    break;
                case NSMDELETE_PO_INTF:
                    printf("\tDelete Po : %s \n", msg->poId);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_DESC:
                case NSMUPDATE_PO_DELDESC:
                    printf("\tDesc : %s \n", msg->desc);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_STATUS:
                case NSMUPDATE_PO_DELSTATUS:
                    printf("\tFlag : %d \n", msg->flag);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_IGNORE_SPLIT:
                case NSMUPDATE_PO_NO_IGNORE_SPLIT:
                    printf("\tIgnore-Split : %d \n", msg->ignore_split);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_MTU:
                case NSMUPDATE_PO_DELMTU:
                    printf("\tMtu : %d \n", msg->mtu); 
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW:
                case NSMUPDATE_PO_DELSW:
                    printf("\tSw %d \n",msg->swModeFlag);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_MODE:
                case NSMUPDATE_PO_DELMODE:
                    printf("\tsw Mode : %d \n", msg->swMode); 
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW_ADD_ACCESS_VLAN:
                case NSMUPDATE_PO_SW_DELETE_ACCESS_VLAN:
                    printf("\tAdd Access : %s \n", msg->swVlanName);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW_TRUNK_VLAN_ALL:
                case NSMUPDATE_PO_SW_TRUNK_VLAN_NONE:
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW_ADD_TRUNK_VLAN:    
                    printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW_DELETE_TRUNK_VLAN:
                    printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                    status = 1; 
                    break;
                case NSMUPDATE_PO_SW_EXCEPT_TRUNK_VLAN:    
                    printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                    status = 1; 
                    break;
                default:
                        status = 1;    
                        printf("\tDesc : %s \n", msg->desc);
                        printf("\tShutdown Flag : %d\n", msg->flag);
                        printf("\tIfIndex : %u\n", msg->poIfIndex);

                        printf("\tSwModeFlag : %d\n", msg->swModeFlag);
                        printf("\tSwMode : %d\n", msg->swMode);
                        printf("\tMtu : %d\n", msg->mtu);
                        printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                        printf("\tNative vlan : %d \n", msg->native_vlan_id);

            }

        }
    }

    printf("status %d \n", status);
		
    return (status);
}

int printPhyIntf(const void *inp, void ** const out)
{
    NsmPhyIntfMsg_t *msg = (NsmPhyIntfMsg_t *)inp;
    int status = 0;
	// unsigned int exception = 0;

    if (msg == NULL)
        return (-1);

    printf("NSMUPDATE_PHY_BULK_CONF ::%d \n",NSMUPDATE_PHY_BULK_CONF);
    printf("Phy cmdCode :%d\n", msg->cmdCode);
    printf("\tName : %s\n", msg->ifName);

    if(msg->cmdCode == NSMUPDATE_PHY_BULK_CONF)
    {
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_DESC_CMD)) {
            printf("\tDesc : %s \n", msg->desc);
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_STATUS_CMD)) {
            printf("\tFlag : %d \n", msg->flag);
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_MTU_CMD)) {
            printf("\tMtu : %d \n", msg->mtu); 
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_LINESPEED_CMD)) {
            printf("\tLineSpeed : %d \n", msg->line_speed); 
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_NATIVEVLANID_CMD)) {
            printf("\tNative vlan : %d \n", msg->native_vlan_id); 
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_CMD)) {
            printf("\tSw \n");
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_MODE_CMD)) {
            printf("\tsw Mode : %d \n", msg->swMode); 
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD)) {
            printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
            printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  

        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD)) {
            printf("\tAdd Access : %s \n", msg->swVlanName);
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL_CMD)) {
            printf("\ttrunk all  \n");
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD)) {
            printf("\tadd trunk vlan : %s \n", msg->swVlanName);
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD)) {
            printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
        }
        if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN_CMD)) {
            printf("\texcept trunk vlan : %s \n", msg->swVlanName);
        }

    } else {
            switch(msg->cmdCode) {
                case NSMUPDATE_PHY_DESC:
                case NSMUPDATE_PHY_DELDESC:
                    printf("\tDesc : %s \n", msg->desc);
                    break;
                case NSMUPDATE_PHY_STATUS:
                    printf("\tFlag : %d \n", msg->flag);
//                    dcm_nsm_notify_portrole_change (msg->ifName, HAL_IF_CAP_EDGE, exception, 101); 
                    break;
                case NSMUPDATE_PHY_DELSTATUS:    
                    printf("\tFlag : %d \n", msg->flag);
//                    dcm_nsm_notify_portrole_change (msg->ifName, HAL_IF_CAP_ISL, exception, 100);
                    break;
                case NSMUPDATE_PHY_MTU:
                case NSMUPDATE_PHY_DELMTU:
                    printf("\tMtu : %d \n", msg->mtu); 
                    break;
                case NSMUPDATE_PHY_LINESPEED:    
                    printf("\tLineSpeed : %d \n", msg->line_speed); 
                    break;
                case NSMUPDATE_PHY_NATIVEVLANID:    
                    printf("\tNative vlan : %d \n", msg->native_vlan_id); 
                    break;
                case NSMUPDATE_PHY_SW:
                case NSMUPDATE_PHY_DELSW:
                    printf("\tSw %d \n",msg->swModeFlag);
                    break;
                case NSMUPDATE_PHY_MODE:
                case NSMUPDATE_PHY_DELMODE:
                    printf("\tsw Mode : %d \n", msg->swMode); 
                    break;
                case NSM_ADD_CHANNELGROUP:
                    printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  
                //  status = 1;
                    break;
                case NSM_DELETE_CHANNELGROUP:    
                    printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  
                    break;
                case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN: 
                case NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN:    
                    printf("\tAdd Access : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:    
                case NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE:    
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    break;
                case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:    
                    printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:    
                    printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
                    printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                    break;
                case NSM_PLUGIN_EDGE_TO_ISL_RESP:
                    printf("\tReceived NSM_PLUGIN_EDGE_TO_ISL_RESP : %d \n", msg->msgId);
                    break;
                case NSM_PLUGIN_ISL_TO_EDGE_RESP:
                    printf("\tReceived NSM_PLUGIN_ISL_TO_EDGE_RESP : %d \n", msg->msgId);
                    break;
                case GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
                case GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
                case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_ADDRESS:
                case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_ADDRESS:
					printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->mac[0], msg->mac[1], msg->mac[2], msg->mac[3], msg->mac[4], msg->mac[5]);
					status = WRC_WAVE_MESSAGE_SUCCESS;
                    break;
                case GVLAN_ACCESS_PORT_SET_MAC_GROUP:
                case GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
                case RSPAN_GVLAN_ACCESS_PORT_SET_MAC_GROUP:
                case RSPAN_GVLAN_ACCESS_PORT_DEL_MAC_GROUP:
					printf("macGroupId:%d.\n", msg->macGroupId);
					status = WRC_WAVE_MESSAGE_SUCCESS;
                    break;
                case GVLAN_TRUNK_PORT_SET_CTAG:
                case GVLAN_TRUNK_PORT_DEL_CTAG:
                case GVLAN_PVLAN_TRUNK_PORT_SET_CTAG:
                case GVLAN_PVLAN_TRUNK_PORT_DEL_CTAG:
                case RSPAN_GVLAN_TRUNK_PORT_SET_CTAG:
                case RSPAN_GVLAN_TRUNK_PORT_DEL_CTAG:
                    printf("vlanId:%s.\n", msg->swVlanName);
					printf("ctagId:%d.\n", msg->ctagId);
					status = WRC_WAVE_MESSAGE_SUCCESS;
                    break;
                default:
                    printf("\tIfIndex : %u\n", msg->swIfIndex);
                    printf("\tDesc : %s \n", msg->desc);
                    printf("\tShutdown Flag : %d\n", msg->flag);

                    printf("\tSwModeFlag : %d\n", msg->swModeFlag);
                    printf("\tSwMode : %d\n", msg->swMode);
                    printf("\tMtu : %d\n", msg->mtu);
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    printf("\tLineSpeed : %d \n", msg->line_speed);
                    printf("\tNative vlan : %d \n", msg->native_vlan_id);
                    printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);
                    break;
            }

        }
            
    *out = NULL;

    if(errorEnable)
    {
        if(msg->bulkCmdCode == NSMUPDATE_PHY_BULK_CONF)
        {
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_DESC_CMD)) {
                printf("\tDesc : %s \n", msg->desc);
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_STATUS_CMD)) {
                printf("\tFlag : %d \n", msg->flag);
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_MTU_CMD)) {
                printf("\tMtu : %d \n", msg->mtu); 
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_LINESPEED_CMD)) {
                printf("\tLineSpeed : %d \n", msg->line_speed); 
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_NATIVEVLANID_CMD)) {
                printf("\tNative vlan : %d \n", msg->native_vlan_id); 
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_CMD)) {
                printf("\tSw \n");
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_MODE_CMD)) {
                printf("\tsw Mode : %d \n", msg->swMode); 
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_ADD_CHANNELGROUP_CMD)) {
                printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  
                status = 1;

            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN_CMD)) {
                printf("\tAdd Access : %s \n", msg->swVlanName);
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL_CMD)) {
                printf("\ttrunk all  \n");
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN_CMD)) {
                printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN_CMD)) {
                printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                status = 1;
            }
            if (IS_CMDCODESET(msg->bulkCmdCode, NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN_CMD)) {
                printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                status = 1;
            }

        } else {
            switch(msg->cmdCode) {
                case NSMUPDATE_PHY_DESC:
                case NSMUPDATE_PHY_DELDESC:
                    printf("\tDesc : %s \n", msg->desc);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_STATUS:
                case NSMUPDATE_PHY_DELSTATUS:    
                    printf("\tFlag : %d \n", msg->flag);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_MTU:
                case NSMUPDATE_PHY_DELMTU:
                    printf("\tMtu : %d \n", msg->mtu); 
                    status = 1;
                    break;
                case NSMUPDATE_PHY_LINESPEED:    
                    printf("\tLineSpeed : %d \n", msg->line_speed); 
                    status = 1;
                    break;
                case NSMUPDATE_PHY_NATIVEVLANID:    
                    printf("\tNative vlan : %d \n", msg->native_vlan_id); 
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW:
                case NSMUPDATE_PHY_DELSW:
                    printf("\tSw %d \n",msg->swModeFlag);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_MODE:
                case NSMUPDATE_PHY_DELMODE:
                    printf("\tsw Mode : %d \n", msg->swMode); 
                    status = 1;
                    break;
                case NSM_ADD_CHANNELGROUP:
                     printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  
                 // status = 1;
                    break;
                case NSM_DELETE_CHANNELGROUP:    
                    printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);  
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW_ADD_ACCESS_VLAN: 
                case NSMUPDATE_PHY_SW_DELETE_ACCESS_VLAN:    
                    printf("\tAdd Access : %s \n", msg->swVlanName);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW_TRUNK_VLAN_ALL:    
                case NSMUPDATE_PHY_SW_TRUNK_VLAN_NONE:    
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW_ADD_TRUNK_VLAN:    
                    printf("\tadd trunk vlan : %s \n", msg->swVlanName);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW_DELETE_TRUNK_VLAN:    
                    printf("\tdelete trunk vlan : %s \n", msg->swVlanName);
                    status = 1;
                    break;
                case NSMUPDATE_PHY_SW_EXCEPT_TRUNK_VLAN:
                    printf("\texcept trunk vlan : %s \n", msg->swVlanName);
                    status = 1;
                    break;
                case NSM_PLUGIN_EDGE_TO_ISL_RESP:
                    printf("\tReceived NSM_PLUGIN_EDGE_TO_ISL_RESP : %d \n", msg->msgId);
                    status = 1;
                    break;
                case NSM_PLUGIN_ISL_TO_EDGE_RESP:
                    printf("\tReceived NSM_PLUGIN_ISL_TO_EDGE_RESP : %d \n", msg->msgId);
                    status = 1;
                    break;
                default:
                    status = 1;
                    printf("\tIfIndex : %u\n", msg->swIfIndex);
                    printf("\tDesc : %s \n", msg->desc);
                    printf("\tShutdown Flag : %d\n", msg->flag);

                    printf("\tSwModeFlag : %d\n", msg->swModeFlag);
                    printf("\tSwMode : %d\n", msg->swMode);
                    printf("\tMtu : %d\n", msg->mtu);
                    printf("\tisAllowedVlanAll : %d\n", msg->isAllowedVlanAll);
                    printf("\tLineSpeed : %d \n", msg->line_speed);
                    printf("\tNative vlan : %d \n", msg->native_vlan_id);
                    printf("\tchannelgroup  : %s %d %d \n", msg->poName, msg->poMode, msg->poType);
                    printf("\tDomainId : %d , Mac : %s \n", msg->poLacpSystemIdDomainId, msg->polacpSystemId);
                    break;
            }

        }
    }
    printf("status : %d \n",status);  
    return (status);

}

int printShowSfp(const void *inp, void ** const out)
{
    SfpShowOutputMsg_t *resp_p = NULL;
    SfpShowOutputRecord_t *ptr = NULL;
    uint32_t i = 0;
    uint32_t size = 0;

    printf("RCVD Show Sfp command\n");

    size = sizeof(SfpShowOutputMsg_t) + (4 * sizeof(SfpShowOutputRecord_t));

    resp_p = (SfpShowOutputMsg_t *)calloc(1, size);

    resp_p->num_records = 5;

    for (i = 0; i < 5; i++) {
        ptr = (SfpShowOutputRecord_t *)&(resp_p->sfp[i]);
        ptr->linecard = i;;
        ptr->port = i;;
        strncpy((char*)ptr->sfpType, "QSFP", 32);
        strncpy((char*)ptr->breakoutType, "4x10G", 32);
        strncpy((char*)ptr->speed, "40G", 32);
    }

    *out = (void *)resp_p;

    return 0;
}

int show_vlan_brief_handler(const void *inp, void ** const outp) {

    NsmShowIntf_InputMsg_t *in = (NsmShowIntf_InputMsg_t *) inp;
    char *op ;
    NsmShowVlanBr_OutputMsg_t *msg;
    showVlanBrVlanRecord_t *vlanPtr = NULL;

    int num_records;
    size_t size;

    printf("SHOW_VLAN_BRIEF: cmdCode=%u, startId=%u, endId=%u, ifName=%s\n",
            in->cmdCode, in->startId, in->endId, in->ifName);
    
    u_int32_t bufferSize = sizeof(NsmShowVlanBr_OutputMsg_t) 
        + 9*(sizeof(showVlanBrVlanRecord_t)) 
        + 49*(sizeof(showVlanBrPortRecord_t)) 
        + 199*(sizeof(gvlan_classification_value_t));

    NsmShowVlanBr_OutputMsg_t* out = (NsmShowVlanBr_OutputMsg_t*) calloc(1,bufferSize);
    out->bufferSize = bufferSize;
    out->num_records = 10;
    out->record_type = NSMSHOW_VLAN_BRIEF;
    out->pageResp.isMore = 0;
    out->pageResp.totalRecordsInBackend = 100;

    num_records = 1 + 3; // number of vlans
    size = sizeof(int) + (sizeof(NsmShowVlanBr_OutputMsg_t) * num_records);

    *outp = op = (char *) calloc(1, size);
    memcpy(op, &num_records, sizeof(int));

    msg = (NsmShowVlanBr_OutputMsg_t *) (op + sizeof(int));

    msg->num_records = num_records;
    msg->record_type = NSMSHOW_VLAN_BRIEF;



    for (short i = 0; i < 3; ++i) {
        msg[i].record_type = NSMSHOW_VLAN_BRIEF;
        vlanPtr = (showVlanBrVlanRecord_t *)&msg->vlan_rec[0];
        vlanPtr->vid = i;
        snprintf(vlanPtr->vlanName, DCM_NSM_VLAN_NAME_MAX, "VLAN%04d", (i+1) );
        printf("record[%d]=%s\n", i-1, vlanPtr->vlanName);
    }

    return 0;
}
int printShowIntf(const void *inp, void ** const out)
{
    /*
    NsmShowIntf_InputMsg_t *msg = (NsmShowIntf_InputMsg_t *)inp;
    NsmShowIntf_OutputMsg_t pOutput;

    if (msg == NULL)
        return (-1);

        printf("Name : %s\n", msg->ifName);
        printf("cmdCode :%d\n", msg->cmdCode);

        sprintf(pOutput.ifName,"%s-%d",msg->ifName,1);
        strcpy (pOutput.desc, msg->ifName);
        pOutput.inpkts      = 1000;

    *out = (void *) &pOutput;
    */

    /*
     * NsmShowIntf_Msg_t *buf;
     */

    
    unsigned char *data;
    int no_of_interfaces, i , len;
    NsmShowIntf_InputMsg_t *msg = (NsmShowIntf_InputMsg_t *)inp;
    NsmShowIntf_OutputMsg_t **pOutput;

    printf("cmdCode :%d\n", msg->cmdCode);

    if (msg->cmdCode == NSMSHOW_VLAN_BRIEF) {
        printf("show vlan brief\n");
        show_vlan_brief_handler(inp, out);
        return 0;
    }

    no_of_interfaces = 0;
    pOutput = (NsmShowIntf_OutputMsg_t **) calloc( no_of_interfaces, sizeof(NsmShowIntf_OutputMsg_t *));

    if (msg == NULL)
        return (-1);

    printf("Name : %s\n", msg->ifName);

    for(i=0; i< no_of_interfaces; i++)
    {
        pOutput[i] = (NsmShowIntf_OutputMsg_t *) calloc( 1, sizeof(NsmShowIntf_OutputMsg_t ));
        sprintf(pOutput[i]->ifName,"%s-%d",msg->ifName,i);
        strcpy (pOutput[i]->desc, msg->ifName);
        pOutput[i]->inpkts      = 1000;
    }

    len = sizeof(NsmShowIntf_OutputMsg_t) * no_of_interfaces;
    data = (unsigned char *)malloc ( sizeof(int) + len);
    *out = (void*) data;
    
    memcpy(data,&no_of_interfaces,sizeof(int));
    data = data + sizeof(int);

    for(i=0; i< no_of_interfaces; i++)
    {
        memcpy(data,pOutput[i],sizeof(NsmShowIntf_OutputMsg_t));
        data = data + sizeof(NsmShowIntf_OutputMsg_t);
    }

     printf("Show Output Data \n");
    for(i=0; i< no_of_interfaces; i++)
    {
        printf("\tName %s \n",pOutput[i]->ifName);
        printf("\tDesc %s \n",pOutput[i]->desc);
        printf("\tInpkts %d \n",pOutput[i]->inpkts);
    }

    for(i=0; i< no_of_interfaces; i++)
    {
        free(pOutput[i]);
    }

    free(pOutput);

    /*
    buf = (NsmShowIntf_Msg_t *)calloc( 1, sizeof(NsmShowIntf_Msg_t) + (no_of_interfaces * sizeof(NsmShowIntf_OutputMsg_t) ));    
    ((NsmShowIntf_Msg_t *) buf)->no_of_interfaces = no_of_interfaces;
    for(i=0; i< no_of_interfaces; i++)
    {
        strcpy (((NsmShowIntf_Msg_t *) buf)->nsmShowMsg[i].desc, pOutput[i]->desc);
        strcpy (((NsmShowIntf_Msg_t *) buf)->nsmShowMsg[i].ifName, pOutput[i]->ifName);
        ((NsmShowIntf_Msg_t) buf)->nsmShowMsg[i].inpkts = pOutput[i]->inpkts;
        printf("Buf Inpkts %d \n",((NsmShowIntf_Msg_t) buf)->nsmShowMsg[i].inpkts);
    } 
    */

    /*
    int no_of_interfaces, i;
    NsmShowIntf_OutputMsg_t **pOutput;
    pOutput = (NsmShowIntf_OutputMsg_t **) calloc( no_of_interfaces, sizeof(NsmShowIntf_OutputMsg_t *)); 
    pOutput[i] = (NsmShowIntf_OutputMsg_t *) calloc( 1, sizeof(NsmShowIntf_OutputMsg_t));
    
    NsmShowIntf_OutputMsg_t pOutput[10];
    no_of_interfaces = 10;

    if (msg == NULL)
        return (-1);

        printf("Name : %s\n", msg->ifName);
        printf("cmdCode :%d\n", msg->cmdCode);

    for(i=0; i< no_of_interfaces; i++)
    {
        sprintf(pOutput[i].ifName,"%s-%d",msg->ifName,i);
        strcpy (pOutput[i].desc, msg->ifName);
        pOutput[i].inpkts      = 1000;
    }

    out = (void ** const) &pOutput;
    */

    return (0);
}

int
nsmFcoeShowHandler (const void *in_p, void ** const out_pp)
{
    fcoe_show_cmd_resp_t    *resp_p = NULL;
    uint32_t                i = 0;
    uint32_t                size = 0;

    printf("RCVD FCOE Eth intf command\n");

    size = sizeof(fcoe_show_cmd_resp_t) +
                    (24 * sizeof(fcoe_show_cmd_ethernet_intf_resp_entry_t));

    resp_p = (fcoe_show_cmd_resp_t *)calloc(1, size);

    resp_p->resp.eth_intf_resp.num_entries = 24;
    resp_p->size = size;
    resp_p->cmd_code = FCOE_SHOW_CMD_CODE_ETHERNET_INTF;

    for (i = 0; i < 24; i++) {
         resp_p->resp.eth_intf_resp.list[i].if_index = i;;
         strncpy((char*)resp_p->resp.eth_intf_resp.list[i].map_name,
                 FCOE_MO_DEFAULT_NAME, FCOE_MAP_NAME_LEN_MAX);
    }

    *out_pp = resp_p;

    return WRC_WAVE_MESSAGE_SUCCESS;
}
int init_call_handlers(int flag) {
    static char *thread_name = (char *) "nsm_dcm_call_handler-thread";
    static pthread_t pthHandler;
    if (flag) 
    pthread_create(&pthHandler, NULL, &nsm_dcm_call_handler, thread_name);
    else 
    pthread_join(pthHandler, NULL);
    return 0;
}

int main(int argc, char **argv)
{
    int ret = 0;

    int slot = 1;
    //int port = 60;

    int j=0;
    int k=0;

    unsigned int exception = 0;
    unsigned int isl_capable = 0;  
    char name[100];
    char description[100];
	int mgmt_port = 9109;

	mgmt_port += atoi(argv[1]);

    if(argc > 2)
    slot = atoi(argv[2]);

    /* Set handler method*/ 
/*  setManagementInterfaceMessageHandlerForClient(); */
    setNSMMangIntfMsgHandlerForClient();

    /* setNSMCmdExecuteHandler( executeNSMCmd); */

    add_client_message_handler(UNIFIEDCLIENTUPDATE, printUnifiedClientUpdate);
    add_client_message_handler(UNIFIEDCLIENTCREATE, printUnifiedClientUpdate);
    add_client_message_handler(UNIFIEDCLIENTDELETE, printUnifiedClientUpdate);
    /* STEP 3 */
    add_client_message_handler(NSMVLANINTF, printVlanIntf);
    add_client_message_handler(NSMPOINTF, printPoIntf);
    add_client_message_handler(NSMPHYINTF, printPhyIntf);
    add_client_message_handler(NSM_SHOW_INTERFACE, printShowIntf);
    add_client_message_handler(SHOW_SFP_MESSAGE, printShowSfp);
    add_client_message_handler(NSMFCOEPORT, printFcoeportAttr);
    add_client_message_handler(NSMFCOEVLAN, printFcoeVlan);
    add_client_message_handler(NSMCLIENTRESERVEDVLAN, printReservedVlan);
    add_client_message_handler(NSM_DEFAULT_CONFIG_CMD, defaultIntfConfHandler);
    add_client_message_handler(FCOE_CLIENT_SHOW, nsmFcoeShowHandler);
    add_client_message_handler(MSG_IP_INT, nsmIpConfigHandler);
    
	add_client_message_handler(NSM_GLOBAL_CONFIG_MSG, printGlobalConfig);
	add_client_message_handler(MSG_VRF_INT, nsm_vrf_config_handler);
    add_client_message_handler(NSMCLIENTCREATESPANLOCALCONFIG, span_config_handler);
    add_client_message_handler(NSM_VLAN_PROVISION, vlan_provision_handler);
    add_client_message_handler(NSM_CLIENT_PVLAN_TRUNK_ASSOC_MSG, pvlan_assoc_handler);
    add_client_message_handler(NSM_CLIENT_PVLAN_MAPPING_MSG, pvlan_mapping_handler);



    /* Register AMPP message handlers.
        */
    add_client_message_handler(APPMPROFILE, portPHandler);
    add_client_message_handler(APPMPORTPROFILEDOMAIN, portProfileDomainHandler);
    add_client_message_handler(APPMVLANPROFILE, vlanPHanlder);
    add_client_message_handler(APPMFCOEPROFILE, fcoePHandler);
	add_client_message_handler(APPMPROFILEBULKMACASSOC, portPHandler);
	add_client_message_handler(PORT_PROFILE_RESTRICT_FLOODING,
			amppRestrictFloodingCommandHandler);
    add_client_message_handler(APPMSHOWMESSAGE,
            amppShowMessageHandler);
    add_client_message_handler(PORT_PROFILE_INTERFACE_ASSOCIATION,
            amppPortProfileOrDomainToInterfaceAssociationHandler);


    /*VlanClassifier Handlers*/

    add_client_message_handler(NSMCLIENTCREATEVLANCLASSIFIERRULESP, printVlanClassifierRulecreate );
    add_client_message_handler(NSMCLIENTDELETEVLANCLASSIFIERRULESP, printVlanClassifierRuledelete );
    add_client_message_handler(NSMCLIENTUPDATEVLANCLASSIFIERGROUPSP, printVlanClassifierGroupCreate );
    add_client_message_handler(NSMCLIENTDELETEVLANCLASSIFIERGROUPSP, printVlanClassifierGroupdelete );
    add_client_message_handler(NSMCLIENTADDVLANCLASSIFIERTOPOSP, printVlanClassifierActivateGroup);
    add_client_message_handler(NSMCLIENTDELETEVLANCLASSIFIERTOPOSP, printVlanClassifierActivateGroup);
    add_client_message_handler(MAC_GROUP_CONFIG_MESSAGE, macGroupConfigMessageHandler);
    add_client_message_handler(UNIFIEDCLIENTGETDATA, populateUnifiedClientGetData);
    add_client_message_handler(SW_VLAN_CLASSIFICATION_MESSAGE, configReplayVlanClassification);
    add_client_message_handler(GVLAN_ENABLE_MESSAGE, gVlanEnableHandler);

	/* add cpStateChange Handler */
	
	add_client_message_handler(NSM_CP_STATE_CHANGE_MSG, cpStateChangeHandler);
	printf ("nsm_app:main cpStateChangeHandler registered\n");

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);
    
    /* Initialize Dcmd */
    ret = dcm_initialize_client ("nsm");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from nsm client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from nsm client... \n");
    } 
    init_call_handlers(1); 

	while (! g_cpActive)
	{
		sleep (5);
	}
		
	printf("HSL: Triggering Postboot programming on global config....\n");
    dcm_nsm_event_notify(SERVICE_READY, 0);
 
    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_EDGE, exception, 100);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/1", HAL_IF_CAP_ISL, exception, 101);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/2", HAL_IF_CAP_ISL, exception, 103);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/3", HAL_IF_CAP_ISL, exception, 101);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/3", HAL_IF_CAP_EDGE, exception, 102);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/3", HAL_IF_CAP_ISL, exception, 103);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_EDGE, exception, 100);
    
    printf("PortRole: Edge_To_ISL....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_ISL, exception, 100);
    
    sleep(12);
    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/1", HAL_IF_CAP_EDGE, exception, 100);
    
    /* Assuming this port is ISL capable */
    isl_capable = 1;
    // NOT USED any more
    
    dcm_nsm_event_notify(LINECARD_ONLINE, 0);
    sleep(5);

    /* Create physical 1G interface */
    slot = 0;
    for (j= slot; j == slot; ++j) {
        for(k = 1; k < 17; k++) {
            sprintf(name, "gi%d/%d", slot, k);
            sprintf(description, "Testing interface: %s", name);
            printf("Interface name: %s ; Description: %s\n", name, description);
            create_phy_interface(name, j, k, description, isl_capable);
        }
    }

    /* Create physical 10G interface */
    //++slot;
    for (j= slot; j == slot; ++j) {
        for(k = 17; k < 33; k++) {
            sprintf(name, "te%d/%d", slot, k);
            sprintf(description, "Testing interface: %s", name);
            printf("Interface name: %s ; Description: %s\n", name, description);
            create_phy_interface(name, j+k, k, description, isl_capable);
        }
    }

    /* Create physical 40G interface */
    //++slot;
    for (j= slot; j == slot; ++j) {
        for(k = 33; k < 49; k++) {
            sprintf(name, "fo%d/%d", slot, k);
            sprintf(description, "Testing interface: %s", name);
            printf("Interface name: %s ; Description: %s\n", name, description);
            create_phy_interface(name, j, k, description, isl_capable);
        }
    }

	create_phy_interface("te0/33:1", 0, 33, "interface te0/33:1", isl_capable);
	create_phy_interface("te0/33:2", 0, 33, "interface te0/33:2", isl_capable);
	create_phy_interface("te0/33:3", 0, 33, "interface te0/33:3", isl_capable);
	create_phy_interface("te0/33:4", 0, 33, "interface te0/33:4", isl_capable);

	create_phy_interface("te0/34:1", 0, 34, "interface te0/34:1", isl_capable);
	create_phy_interface("te0/34:2", 0, 34, "interface te0/34:2", isl_capable);
	create_phy_interface("te0/34:3", 0, 34, "interface te0/34:3", isl_capable);
	create_phy_interface("te0/34:4", 0, 34, "interface te0/34:4", isl_capable);

    printf("HSL: Triggering Postboot programming on linecard config....\n");
	dcm_nsm_event_notify(LINECARD_READY, 0);

    sleep(5);
    dcm_nsm_event_notify(CHASSIS_READY, 0);

    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_EDGE, exception, 100);
    printf("PortRole: Edge_To_ISL....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_ISL, exception, 100);
    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_EDGE, exception, 100);
   
    printf("PortRole: ISL_To_Edge....\n");
    dcm_nsm_notify_portrole_change ("10/0", HAL_IF_CAP_EDGE, exception, 101); 


    printf("No shutdown te 0/21 \n");
    dcm_nsm_notify_phystatus_change ("0/21", NO_SHUTDOWN, 101);
    printf("shutdown  te 0/22 \n");
    dcm_nsm_notify_phystatus_change ("0/22", SHUTDOWN, 100);
    

    {
    nsm_intf_all_msg_t *msg = (nsm_intf_all_msg_t*) malloc(sizeof(nsm_intf_all_msg_t));
    msg->num_records = 1;
    msg->record_type=CHASSIS_ENABLE;
    strcpy(msg->intfdetails[0].ifName,"te0/0");
	dcm_nsm_notify_portrole_change ("0/0", HAL_IF_CAP_ISL, exception, 100);

    dcm_modify_interface_config(msg);
    }

    init_call_handlers(0); 
}

void *nsm_dcm_call_handler(void *vp) {
    char *name = (char *)vp; 
    printf("Starting thread with name as %s\n", name);
    while(1) {
        sleep(5 * 60); //Every five minute make the call;
        printf("Triggering the unprovision\n");
        dcm_nsm_trigger_vlan_unprovision();
    }
    pthread_exit(0);
}

int printVlanClassifierActivateGroup(const void *inp, void ** const out)
{
    VlanClassifierActivateGroupMsg_t *msg = (VlanClassifierActivateGroupMsg_t *)inp;
     if (msg == NULL)
    {
        printf("------>msg is Null\n");
        return (-1);
    }

    printf("PortId : %d\n", msg->PortId);
    printf("groupId : %d\n", msg->groupid);
   // printf("vlanId : %d\n", msg->vlanid);
    printf("PortName : %s\n", msg->PortName);
    printf("opcode : %d\n", msg->opcode);
   // printf("ifIndex : %d\n", msg->ifIndex);

    *out = NULL;

    return (0);



}

int printVlanClassifierGroupCreate(const void *inp, void ** const out)
{
     VlanClassifierCreateGroupMsg_t  *msg = (VlanClassifierCreateGroupMsg_t *)inp;

    if (msg == NULL)
    {
        printf("------>msg is Null\n");
        return (-1);
    }

        printf("ruleId : %d\n", msg->ruleId);
        printf("groupId : %d\n", msg->groupId);
        printf("operation : %d\n", msg->operation);

    *out = NULL;

    return (0);

}

int printVlanClassifierRuledelete(const void *inp, void ** const out)
{
    unsigned int *msg = (unsigned int*)inp;

    if (msg == NULL)
    {
    printf("------>msg is Null\n");
        return (-1);
    }

    printf("ruleId : %d\n", *msg);
    *out = NULL;
    return (0);
}
int printVlanClassifierGroupdelete(const void *inp, void ** const out)
{
    unsigned int *msg = (unsigned int*)inp;

    if (msg == NULL)
    {
    printf("------>msg is Null\n");
        return (-1);
    }

    printf("GroupId : %d\n", *msg);
    *out = NULL;
    return (0);
}

int printVlanClassifierRulecreate(const void *inp, void ** const out)
{
    VlanClassifierCreateRuleMsg_t  *msg = (VlanClassifierCreateRuleMsg_t *)inp;

    if (msg == NULL)
    {
    printf("------>msg is Null\n");
        return (1);
    }

        printf("ruleId : %d\n", msg->ruleId);
        printf("protoval : %d\n", msg->protoVal);
        printf("encap : %d\n", msg->encap);
        printf("mac : %s \n", msg->mac);
        printf("opcode:%d \n",msg->opcode);

    *out = NULL;
//    printf("WRC_VLAN_CLASSIFIER_NSM_API_ERR_INVALID_MAC =%d \n\r", WRC_VLAN_CLASSIFIER_NSM_API_ERR_INVALID_MAC);
//    return (WRC_VLAN_CLASSIFIER_NSM_API_ERR_INVALID_MAC);
    return (0);
}

int nsmIpConfigHandler(const void *inp, void ** const out)
{
    nsm_ip_global_config_msg_t  *msg = (nsm_ip_global_config_msg_t *)inp;

    printf("TestApp.c :: nsmIpConfigHandler() ... \n");

    if (msg == NULL)
    {
        printf("Error: --> msg is Null\n");
        return (-1);
    }

    printf ("%s(), if_name = %s\n", __FUNCTION__, msg->if_name);
    printf ("%s(), if_id = %d\n", __FUNCTION__, msg->if_id);
    printf ("%s(), intf_type = %d\n", __FUNCTION__, msg->intf_type); 
    printf ("%s(), opcode = %d\n", __FUNCTION__, msg->opcode);
    printf ("%s(), ipaddrmask = %s\n", __FUNCTION__, msg->ip_address_mask);
    printf ("%s(), secondary = %d\n", __FUNCTION__, msg->sec);
    printf ("%s(), mtu = %d\n", __FUNCTION__, msg->ip_mtu);
    printf ("%s(), proxy-arp = %d\n", __FUNCTION__, msg->proxy_arp);
    printf ("%s(), art-aging-timeout = %d\n", __FUNCTION__, msg->arp_timeout);
    printf("\n\n");

    *out = NULL;

    return (0);
}


int portPHandler(const void *inp, void ** const out) { 
    printf("TestApp.c :: portPHandler() ... \n");
    printf("\n\n");
    *out = NULL;
    return (0);
}

int portProfileDomainHandler(const void *inp, void** const out) {
    portProfileDomainMessage_t* profileDomainMessage = (portProfileDomainMessage_t*) inp;
    printf("CommandCode<%d>", profileDomainMessage->cmdCode);
    printf("Opcode<%d>", profileDomainMessage->opCode);
    printf("PortProfileDomainName<%s>", profileDomainMessage->portProfileDomainName);
    printf("PortProfileName<%s>", profileDomainMessage->portProfileName);
    return (0);
}

int vlanPHanlder( const void *inp, void ** const out) {
    printf("TestApp.c :: vlanPHandler() ... \n");
    printf("\n\n");

    vlanProfileMsg_t* pvl = (vlanProfileMsg_t*) inp;
    printf("cmdCode<%d>", pvl->cmdCode);
    printf("profileName<%s>", pvl->profileName);
    printf("accessVlan<%d>", pvl->m_accessVlan);
    printf("trunkVlanId<%d>", pvl->trunkVlanId);
    printf("nativeVlanId<%d>", pvl->m_nativeVlan);
    printf("vlanClassificationType<%d>", ((pvl->vlanClassificationType)));
    printf("numberOfVlanClassifications<%d>", pvl->numberOfVlanClassifications);

    switch(((pvl->vlanClassificationType)))
    {
        case APPM_VLAN_NOCLASSIFICATION :
            {
                printf("NO VLAN CLASSIFICATION.");
                break;
            }
        case APPM_VLAN_CTAGCLASSIFICATION :
            {
                printf("CTAG <%d>", pvl->vlanClassificationRecords[0].ctag.ctag);
                break;
            }
        case APPM_VLAN_MACCLASSIFICATION :
            {
                printf("MAC <%s>", pvl->vlanClassificationRecords[0].mac.macAddress);
                break;
            }
        case APPM_VLAN_MACGROUPCLASSIFICATION :
            {
                printf("MACGROUP <%d>", pvl->vlanClassificationRecords[0].macGroup.macGroupId);
                break;
            }
        case APPM_VLAN_VNICLASSIFICATION :
            {
                printf("VNI");
                break;
            }
    }
    *out = NULL ;
    return (0);
}

int fcoePHandler( const void *inp, void ** const out ) {
    printf("TestApp.c :: fcoePHandler() ... \n");
    printf("\n\n");
    *out = NULL ;
    return (0);
}

int amppRestrictFloodingCommandHandler(const void *inp, void ** const out) {
	portProfileMsg_t* ptr = (portProfileMsg_t*) inp;

	printf(
			"amppRestrictFloodingCommandHandler opcode<%d> cmdcode <%d> profilename <%s> profileId<%d> isActivated<%d> isAllow<%d> profileMacStr<%s> isFloodingRestricted<%d> \n",
			ptr->opCode, ptr->cmdCode, ptr->profileName, ptr->profileId,
			ptr->isActivated, ptr->isAllow, ptr->profileMacStr,
			ptr->isFloodingRestricted);
	return 0;
}

int amppShowMessageHandler(const void *inp, void ** const out) {
    AppmShowMsgInpt_t *pInput = (AppmShowMsgInpt_t*)inp;

    printf ("amppShowMessageHandler Command Code : <%d> \n",pInput->cmdCode);
    printf ("amppShowMessageHandler Number of records : <%d> \n",  pInput->numOfRecords);
    printf ("amppShowMessageHandler Last Profile Mac : <%s> \n", pInput->lastProfileMac);
    printf ("amppShowMessageHandler Interface name: <%s> \n", pInput->ifName);
    printf ("amppShowMessageHandler Profile name: <%s> \n", pInput->profileName);
    printf ("amppShowMessageHandler Profile Domain name: <%s> \n", pInput->portProfileDomainName);
    printf ("amppShowMessageHandler Fetch Model Type: <%d> \n",  pInput->fetchModelType);

    return 0;
}

int amppPortProfileOrDomainToInterfaceAssociationHandler(const void *inp, void ** const out) {

    return 0;

}

int macGroupConfigMessageHandler (const void *inp, void ** const out)
{
    MacGroupInputMsg_t *msg = (MacGroupInputMsg_t *)inp;
	unsigned int macRecordIdx = 0;

    printf("nsm_app.c::macGroupConfigMessageHandler() ... \n");

    if (msg == NULL)
    {
        printf("Error: --> msg is Null\n");
        return (-1);
    }

    printf ("%s(), cmdCode		:%d.\n", __FUNCTION__, msg->cmdCode);
    printf ("%s(), macGroupId	:%d.\n", __FUNCTION__, msg->macGroupId);
    printf ("%s(), numMacRecords:%d.\n", __FUNCTION__, msg->numMacRecords);

	for(macRecordIdx = 0; macRecordIdx < msg->numMacRecords; macRecordIdx++)
	{
		MacGroupRecord_t *rec = &(msg->macGroupRecord[macRecordIdx]);
		printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", rec->address[0], rec->address[1], rec->address[2], rec->address[3], rec->address[4], rec->address[5]);
		printf("mask:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", rec->mask[0], rec->mask[1], rec->mask[2], rec->mask[3], rec->mask[4], rec->mask[5]);
	}
    printf("\n\n");

    *out = NULL;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int populateUnifiedClientGetData (const void *inp, void ** const out)
{
	static int isMore = 0;
	char *output = NULL;
	char *input_buffer;
	dcm_tlv_t *tlv_p;
	int tlv_count = 0;
	int j=0;
	int i=0;

	printf("Unified Client Get Data message handler entering.\n");

	if (!inp)
	{   //Return with an error
		printf("Unified Client Get Data message handler received NULL input structure");
		return (1);
	}

	UnifiedClientMessageInput_t *msg = (UnifiedClientMessageInput_t *) inp;
	input_buffer = (char*)msg->inputBuffer;
	printf("Unified Client Get Data message commandType = %d\n", msg->commandType);
	printf("Unified Client message has argc=%d\n", msg->commandStringSize);
	printf("Unified Client message has argv=");

	for(i = 0; i < msg->commandStringSize; i++)
	{
		printf ("%s ", msg->commandString[i]);
	}

	printf("\nDone with printing arguments.\n");

	if (input_buffer != NULL)
	{
		tlv_count = getTLVCountInBuffer(input_buffer);
		printf("tlv count=%d\n", tlv_count);

		if (tlv_count != 0)
		{
			for (j = 0 ; j < tlv_count; j++)
			{
				tlv_p = getTLVFromBuffer (input_buffer, j);

				switch (tlv_p->type)
				{
					case NSM_HEADER:
					{
						printf("Entered NSM_HEADER.\n");
						printf("value=%s\n", tlv_p->value);
					}
					break;

					default:
					{
						printf("Entered default %u\n", tlv_p->type);
					}
					break;
				}
			}
		}
	}
	printf("Unified Client Get Data message handling complete.\n");

	UnifiedClientMessageOutput_t *result = (UnifiedClientMessageOutput_t *) malloc(sizeof(UnifiedClientMessageOutput_t ));

	if (!isMore)
	{
		//first time
		result->moreRecords = 1;
		isMore = 1;
	}
	else
	{
		result->moreRecords = 0;
		isMore = 0;
	}

	switch (msg->groupCode)
	{
#if 0
		case NSM_DEBUG_SHOW_GVLAN: /* debug show gvlan */
		{
			printf("populateUnifiedClientGetData::debug show gvlan.\n");
		}
		break;

		case NSM_DEBUG_SHOW_GVLAN_VLANID: /* debug show gvlan vlan-id */
		{
			printf("populateUnifiedClientGetData::debug show gvlan.\n");
		}
			break;

		case NSM_DEBUG_SHOW_IVID: /* debug show ivid */
		{
			printf("populateUnifiedClientGetData::debug show gvlan.\n");
		}
		break;

		case NSM_DEBUG_SHOW_IVID_VLANID: /* debug show ivid vlan-id */
		{
			printf("populateUnifiedClientGetData::debug show gvlan.\n");
			break;
		}
#endif
	}

	output = (char *)malloc (100);

	if (output != NULL)
	{
		memset(output, 0, 100);
	}

	int err = 0;
	err = dcmTlvBufferInit(output);
	printf("Done with dcmTlvBufferInit.\n");
	char header_value[]="myheader";
	err = appendTLVToBuffer (output, NSM_HEADER, sizeof(header_value), header_value);

	if (err != 0)
	{
		printf("Problem with myheader.\n");
	}

	printf("Done with myheader.\n");
	char data_value[]="hello world\n";
	err = appendTLVToBuffer(output, NSM_DATA, 13, data_value);

	if (err != 0)
	{
		printf("Problem with data.\n");
	}

	printf("Done with hello world.\n");
	result->outputBufferSize = 100;
	result->outputBuffer = output;
	result->bufferTag = 1;

	(*out) = result;
	return (0);
}

int configReplayVlanClassification(const void *inp, void ** const out)
{
    unsigned int count = 0;
    char if_name[65];
    SwitchportVlanClassificationMsg_t *msg = (SwitchportVlanClassificationMsg_t *)inp;

    if (msg == NULL)
        return (-1);

	printf("configReplayVlanClassification:GVlan switchport cmds received, numMacRecords: %d\n", msg->numRecords);

    while(count < msg->numRecords){
        GENERATE_SHORT_IF_NAME_FOR_BACK_END(msg->rec[count].ifType, msg->rec[count].ifName, if_name, 64);
        switch (msg->rec[count].cmdCode) {
            case SW_VLAN_CLASSIFICATION_SET_MAC_ADDR:
                printf("SW_VLAN_CLASSIFICATION_SET_MAC_ADDR:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macAddressRec.vlanId);
				printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->rec[count].payload.macAddressRec.mac[0], msg->rec[count].payload.macAddressRec.mac[1], msg->rec[count].payload.macAddressRec.mac[2], msg->rec[count].payload.macAddressRec.mac[3], msg->rec[count].payload.macAddressRec.mac[4], msg->rec[count].payload.macAddressRec.mac[5]);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_MAC_ADDR:
                printf("SW_VLAN_CLASSIFICATION_DEL_MAC_ADDR:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macAddressRec.vlanId);
				printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->rec[count].payload.macAddressRec.mac[0], msg->rec[count].payload.macAddressRec.mac[1], msg->rec[count].payload.macAddressRec.mac[2], msg->rec[count].payload.macAddressRec.mac[3], msg->rec[count].payload.macAddressRec.mac[4], msg->rec[count].payload.macAddressRec.mac[5]);
                break;

            case SW_VLAN_CLASSIFICATION_SET_MAC_GROUP:
                printf("SW_VLAN_CLASSIFICATION_SET_MAC_GROUP:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macGroupRec.vlanId);
				printf("macGroupId:%d.\n", msg->rec[count].payload.macGroupRec.macGroupId);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_MAC_GROUP:
                printf("SW_VLAN_CLASSIFICATION_DEL_MAC_GROUP:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macGroupRec.vlanId);
				printf("macGroupId:%d.\n", msg->rec[count].payload.macGroupRec.macGroupId);
                break;

            case SW_VLAN_CLASSIFICATION_SET_CTAG:
                printf("SW_VLAN_CLASSIFICATION_SET_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_CTAG:
                printf("SW_VLAN_CLASSIFICATION_DEL_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            case SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG:
                printf("SW_VLAN_CLASSIFICATION_SET_PVLAN_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_PVLAN_CTAG:
                printf("SW_VLAN_CLASSIFICATION_DEL_PVLAN_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            case SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_ADDR:
                printf("SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_ADDR:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macAddressRec.vlanId);
				printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->rec[count].payload.macAddressRec.mac[0], msg->rec[count].payload.macAddressRec.mac[1], msg->rec[count].payload.macAddressRec.mac[2], msg->rec[count].payload.macAddressRec.mac[3], msg->rec[count].payload.macAddressRec.mac[4], msg->rec[count].payload.macAddressRec.mac[5]);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_ADDR:
                printf("SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_ADDR:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macAddressRec.vlanId);
				printf("mac:%.2hx%.2hx.%.2hx%.2hx.%.2hx%.2hx:\n", msg->rec[count].payload.macAddressRec.mac[0], msg->rec[count].payload.macAddressRec.mac[1], msg->rec[count].payload.macAddressRec.mac[2], msg->rec[count].payload.macAddressRec.mac[3], msg->rec[count].payload.macAddressRec.mac[4], msg->rec[count].payload.macAddressRec.mac[5]);
                break;

            case SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_GROUP:
                printf("SW_VLAN_CLASSIFICATION_SET_RSPAN_MAC_GROUP:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macGroupRec.vlanId);
				printf("macGroupId:%d.\n", msg->rec[count].payload.macGroupRec.macGroupId);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_GROUP:
                printf("SW_VLAN_CLASSIFICATION_DEL_RSPAN_MAC_GROUP:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.macGroupRec.vlanId);
				printf("macGroupId:%d.\n", msg->rec[count].payload.macGroupRec.macGroupId);
                break;

            case SW_VLAN_CLASSIFICATION_SET_RSPAN_CTAG:
                printf("SW_VLAN_CLASSIFICATION_SET_RSPAN_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            case SW_VLAN_CLASSIFICATION_DEL_RSPAN_CTAG:
                printf("SW_VLAN_CLASSIFICATION_DEL_RSPAN_CTAG:Index(%d):\n", count);
				printf("ifname:%s, Vlan:%d.\n", if_name, msg->rec[count].payload.ctagRec.vlanId);
				printf("ctagId:%d.\n", msg->rec[count].payload.ctagRec.ctagId);
                break;

            default:
				printf("configReplayVlanClassification:Invalid Command code:%d:received on Index(%d) for ifName:%s.\n", msg->rec[count].cmdCode, count, if_name);
                break;
        }
        count++;
    }

    *out = NULL;

    return WRC_WAVE_MESSAGE_SUCCESS;
}

int returnErrorForGVlan()
{
	static int returnError = 1;
	returnError++;

	if (returnError%4 == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}

int gVlanEnableHandler(const void *inp, void ** const out)
{
	gvlan_enable_input_msg_t *msg = (gvlan_enable_input_msg_t *)inp;
	unsigned int status = WRC_WAVE_MESSAGE_SUCCESS;

	if (msg == NULL)
		return (-1);

	gvlan_enable_stage_t stage = msg->stage;

	if(msg->enable)
		printf("gVlanEnableHandler:GVlan Enable received.\n");
	else
		printf("gVlanEnableHandler:GVlan Disable received.\n");

	if(msg->undo)
		printf("gVlanEnableHandler:GVlan Undo received.\n");

	switch(stage)
	{
		case GVLAN_ENABLE_STG_1:
			printf("cmdCode:GVLAN_ENABLE_STG_1.\n");
			break;
		case GVLAN_ENABLE_STG_2:
			printf("cmdCode:GVLAN_ENABLE_STG_2.\n");
			if(returnErrorForGVlan())
			{
				status = WRC_WAVE_MESSAGE_ERROR;
			}
			break;
		case GVLAN_ENABLE_STG_3:
			printf("cmdCode:GVLAN_ENABLE_STG_3.\n");
			break;
		case GVLAN_ENABLE_STG_4:
			printf("cmdCode:GVLAN_ENABLE_STG_4.\n");
			break;
		default:
			printf("cmdCode:UNKNOWN.\n");
			break;
	}

	*out = NULL;

	return status;
}
