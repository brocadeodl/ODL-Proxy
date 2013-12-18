#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/vCenter/vCenterClientHandler.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/vCenter/vCenterEventMessage.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "../source/ValClientInterface/ValClientSynchronousConnection.h"
#include "Framework/Utils/Integer.h"

#include <netinet/in.h>
#include "DcmCStatus.h"
#include "WaveCStatus.h"

#include <assert.h>
using namespace DcmNs;
//int glret = WRC_WAVE_MESSAGE_SUCCESS;

int main(int argc, char *argv[]) {
    int ret = 0;
    int mgmt_port = 9109;

    if (argc < 2) {
        printf("usage: vcenter_app.Linux.i686 mgmt_port vm_mor listen\n");
        return 0;
    }
    mgmt_port += atoi(argv[1]);

    //setvCenterMsgHandlerForClient();

    //add_client_message_handler(VCENTEREVENTS, handlevCenterEvents);

    WaveNs
    ::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    ret = dcm_initialize_client_for_dual_daemons("vCenterDcmClient89");
    /*vnm_req_t *req = (vnm_req_t *) malloc(sizeof(vnm_req_t));
    req->num_plds = 1;
    req->req_type = VNM_REQ_VM_POWER_STATUS_USING_COOKIE;
    req->payload.cookie[0].mig_task_id = 1265;
    struct vnm_ip_addr_t vm_ip;
    convert_string_to_ip(&vm_ip,"10.32.45.67");
    req->payload.cookie[0].vcenter_ip =vm_ip;
    strcpy(req->payload.cookie[0].vm_id, argv[2]);


    uint32_t message_id;
    vnm_resp_t *vnm_resp;
    vnm_request(*req, &message_id, &vnm_resp);
    if (vnm_resp) {

        printf("\nvCenter IP %s\n",convert_ip_to_string(vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vcenter_ip));

        //Success
        if(vnm_resp->msg.payload.vm_pwd_pld[0].ret_code == VNM_ERR_SUCCESS){
        printf("VM-MOR %s\n",(vnm_resp->msg.payload.vm_pwd_pld[0].cookie.vm_id));

        if(vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status==VMM_VM_POWERED_ON){
            printf("Retrieving Power Status Completed %d %s\n",
                    vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,"VM Powered ON" );
        }else if(vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status==VNM_VM_POWERED_OFF){
            printf("Retrieving Power Status Completed %d %s\n",
                                vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,"VM Powered OFF" );
        }else if(vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status==VNM_VM_SUSPENDED){
            printf("Retrieving Power Status Completed %d %s\n",
                                vnm_resp->msg.payload.vm_pwd_pld[0].vnm_power_status,"VM Suspended" );
        }
        }else{
            printf("Retrieving Power Status Failed : Return Code = %d\n",
                    vnm_resp->msg.payload.vm_pwd_pld[0].ret_code);
        }


    }
*/

 		//ResourceId status = WAVE_MESSAGE_SUCCESS;
        //SI32 clientStatus = 0;
        //UI32 msg_id = 0;
        //TODO clean up printf

        if (true != (ValClientSynchronousConnection::isSynchronousConnectionForCcmInitialized())) {
            trace(TRACE_LEVEL_INFO,
            "Fail to ValClientSynchronousConnection::isSynchronousConnectionForCcmInitialized ()\n");

    //        return (-1);
        }

        ValClientSynchronousConnection *pValClientSynchronousConnection = ValClientSynchronousConnection::getValClientSynchronousConnectionForDualDaemons();

        prismAssert(NULL != pValClientSynchronousConnection, __FILE__,
                __LINE__);	
   

    if (ret != 0) {
        printf(
                "Failed :: Dcm-client Initialization failed from vCenter client... \n");
        assert(ret==0);
    }
    else {
        printf(
                "Passed :: Dcm-Client Initialization passed from vCenter client... \n");
    }
	UI32 allowedVlanID = 19;
	UI32Range allowedVlanRange ((Integer (allowedVlanID)).toString ());
	UI32Range vlanCreaterange = UI32Range("");
	string vlanRangeStr;
	vector <UI32> vlanCreateVector;
	vector <string> macVector;
	vlanCreateVector.push_back(5);
	vlanCreateVector.push_back(15);
	vlanCreateVector.push_back(25);
	vlanCreateVector.push_back(35);
	vlanRangeStr = UI32Range::getUI32RangeStringFromVector(vlanCreateVector);
	vlanCreaterange.fromString(vlanRangeStr); 
	MacAddress  macAddr ("00:1E:0B:BF:47:D4");
	MacAddress  macAddr1 ("00:1E:0B:BF:42:D4");
	MacAddress  macAddr2 ("00:1E:0B:BF:41:D4");
	MacAddress  macAddr3 ("00:1E:0B:BF:48:D4");
	macVector.push_back(macAddr1.toString2());
	macVector.push_back(macAddr2.toString2());
	macVector.push_back(macAddr3.toString2());

 pValClientSynchronousConnection->createPortProfile("test19");
 pValClientSynchronousConnection->createPortProfile("test29");
 pValClientSynchronousConnection->createAccessVlanProfileWithVlanForPortProfile("test29",9);
 pValClientSynchronousConnection->createTrunkVlanProfileWithVlanForPortProfile("test19",vlanCreaterange);
 pValClientSynchronousConnection->activatePortProfile("test19");
 pValClientSynchronousConnection->associateMacToVcenterPortProfile("test19",macAddr.toString2());

 pValClientSynchronousConnection->activatePortProfile("test29");
 pValClientSynchronousConnection->associateMacsToPortProfile("test29",macVector);

 pValClientSynchronousConnection->disassociateMacFromVcenterPortProfile("test29",macAddr2.toString2());

 //pValClientSynchronousConnection->deleteVlanProfileForPortProfile("test19");
// pValClientSynchronousConnection->deleteVlanProfileForPortProfile("test39");

// pValClientSynchronousConnection->createQoSProfileWithDefaultCOSForPortProfile("test9",4);
//pValClientSynchronousConnection->deleteQoSProfileForPortProfile("test9");
 
    if(argc==4)
    while (1)
        sleep(100);
}
