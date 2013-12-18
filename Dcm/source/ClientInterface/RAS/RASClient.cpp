/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                    *
 ***************************************************************************/

#include <iostream>
#include <strings.h>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/WaveCliShell.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"

#include "ValClientInterface/DcmClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/RAS/RASClient.h"
#include "ClientInterface/RAS/RASClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{



// ######## C style to receive Wave message ####################
// int app_vcslogging_msg_handler(const void *inp, void ** const out)

// app_main() {
// Dcmd-Plugin-Step:1  Call Client interface instanciator */
// setRASMsgHandlerForClient();
	
// Dcmd-Plugin-Step:2  Initialize Message Handler */
// add_client_message_handler(RAS_VCSLOGGING, app_vcslogging_msg_handler);

// Dcmd-Plugin-Step:3  Register as Wave/Dcmd  Client */
// rc = dcm_initialize_client ("RAS");
// }

ManagementInterfaceMessage *RASMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode) {
        case LCCLI_MSG:
            pDcmManagementInterfaceMessage =  new LCCLIMsg();
	    trace (TRACE_LEVEL_INFO, string ("RASMsgInstantiator : LCCLI"));
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("RASMsgInstantiator : Unknown Operation Code : ") +
		   operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int
lccli_cb (const void *inp, void ** const out)
{
    int ret = -1;
    lccli_in_msg_t *in_msg = (lccli_in_msg_t *) inp;
    lccli_out_msg_t *out_msg = NULL;
    FILE	*cmd_op = NULL;
    char	cmd_output[513];
    string data = "";
    char cmdline[512];
    int rc = 0;

    if (!in_msg)
      return (ret);

    printf("Receive LCCLI=%d cmd=%s\n", in_msg->cli_code, in_msg->argv);
    switch (in_msg->cli_code) {
    case LCCLI_MSG:
	snprintf(cmdline, 512, "%s 2>/dev/null", (char *)in_msg->argv);
	if ((cmd_op = popen(cmdline, "r")) != NULL) {
	    while (fgets(cmd_output, 128, cmd_op)) {
		    // printf("Data=%s\n", cmd_output);
		data.append(cmd_output);
	    }
	} else {
            printf("Fail to popen CLI\n");
	    data.append(string ("Fail to popen CLI:") + in_msg->argv + string ("\n"));
	    break;
	}

	if ((rc = pclose(cmd_op)) < 0) {
            printf("Fail to pclose CLI rc=%d\n", rc);
	    data.append (string ("Fail to pclose CLI:") + in_msg->argv + string("Error:") + rc);
	    break;
	}
        break;

	default :
		break;
    }

    // DcmClient.cpp dcmCStyleClientDcmManagementInterfaceMessageHandler() will free it 
    // data size need to be sizeof (lccli_out_msg_t) + real data size 
    out_msg = (lccli_out_msg_t *)calloc(1, sizeof (lccli_out_msg_t) + data.size());
    if(!out_msg) {
	*out = NULL;
	return (-1);
    }

    strcpy((char *)out_msg + sizeof (lccli_out_msg_t), data.c_str());
    out_msg->out_buf_len = data.size();

    printf("Reply data len=%lu\n", (long unsigned int) data.size());
    *out = out_msg;

    return 0;
}


// Setup Cstyle code to receive CPP msg
int setRASMsgHandlerForClient()
{
     int status = 1 ;
 
     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (RASMsgInstantiator);
     add_client_message_handler(LCCLI_MSG, lccli_cb);

     return status;
}


void setManagementInterfaceReceiverPort (int serverPort)
{ 
   FrameworkToolKit::setManagementInterfaceReceiverPort (serverPort);
}

// ### C function to be used by DCMCLient to send Wave message to DCMServfer PI ###
int send_vcslog (vcslog_t *vcslog)
{ 
    ResourceId status                          = WAVE_MESSAGE_SUCCESS;
    SI32       clientStatus                    = 0;
    string     vcslogString = "sw";

    DcmClientSynchronousConnection *pDcmClientSynchronousConnection = DcmClientSynchronousConnection ::getDcmClientSynchronousConnectionForDualDaemons();

    prismAssert (NULL != pDcmClientSynchronousConnection, __FILE__, __LINE__);

    status = pDcmClientSynchronousConnection->vcsRASLogging (vcslog, vcslogString);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        clientStatus = 0;
    }
    else
    {
	printf ("Fail to DcmClientSynchronousConnection::rc=%d\n", status);
        clientStatus = -1;
    }

    return (clientStatus);
}

}
