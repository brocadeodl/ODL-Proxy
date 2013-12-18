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

#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/Lccli/LccliClient.h"
#include "ClientInterface/Lccli/LccliClientMessage.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;


extern "C"
{

static const char *debugusage = {
" Below is the list of suported CLIs. There might be more suported\n"
" CLIs that are not listed here. A new CLI can be updated to the list\n"
" by adding CLI path to the file located at\n"
" /fabos/src/vcs/dcm/source/ClientInterfac/Lccli/LccliClient.c\n"
"===========================================\n"
"chkconfig \n"
"rasdecode\n"
"/fabos/bin/errdump \n"
"/fabos/cliexec/errdumpall\n"
"/fabos/cliexec/errclear\n"
"/fabos/cliexec/errlogtest\n"
"/fabos/cliexec/errlogutest\n"
"/fabos/cliexec/errmoduleshow\n"
"/fabos/cliexec/version\n"
"/fabos/sbin/coreshow \n"
"/fabos/sbin/db \n"
"/fabos/sbin/supportsave\n"
"/fabos/sbin/supportuffdc\n"
"/fabos/sbin/tracedump\n"
"/fabos/sbin/tracestore\n"
"/fabos/sbin/oscmd\n"
};


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

ManagementInterfaceMessage *LccliMsgInstantiator (const UI32 &operationCode)
{
    DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

    switch (operationCode) {
        case LCCLI_MSG_CODE:
            pDcmManagementInterfaceMessage =  new LccliMsg();
	        trace (TRACE_LEVEL_INFO, string ("LccliMsgInstantiator : LCcli"));
            break;
        default:
            trace (TRACE_LEVEL_INFO, string ("LccliMsgInstantiator : Unknown Operation Code : ") +
		         operationCode);
            pDcmManagementInterfaceMessage = NULL;
    }                                                                                                                
    return (pDcmManagementInterfaceMessage);
}

int
lccli_cb_handler (const void *inp, void ** const out)
{
    lccli_input_msg_t *in_msg = (lccli_input_msg_t *) inp;
    lccli_output_msg_t *out_msg = NULL;
	string data = "";
	FILE	*cmd_op = NULL;
    char	cmd_output[513];
    char cmdline[512];
	int rc = 0;

    if (!in_msg) {
	  tracePrintf (TRACE_LEVEL_ERROR, "lccli_cb_handler: null pointer\n");
      return (-1);
	}
	tracePrintf(TRACE_LEVEL_INFO, "lcclid: lccli_cb_handler cmd %s cmdcode %d\n", in_msg->argv, in_msg->cli_code);
    switch (in_msg->cli_code) {
    case LCCLI_MSG_CODE:
	   if (strcmp(in_msg->argv, "cli-help") == 0) {
	      data.append(debugusage);
	   } else {
	      snprintf(cmdline, 512, "%s 2>/dev/null", (char *)in_msg->argv);
	      if ((cmd_op = popen(cmdline, "r")) != NULL) {
	        while (fgets(cmd_output, 128, cmd_op)) {
		      data.append(cmd_output);
	        }
	        data.append(string("\n"));
	      } else {
	        data.append(string ("Fail to popen CLI:") + in_msg->argv + string ("\n"));
	      }
	      if ((rc = pclose(cmd_op)) < 0) {
	         data.append (string ("Fail to pclose CLI:") + in_msg->argv +
	                  string("Error:") + rc + string ("\n"));
	      }
	   }
	   out_msg = (lccli_output_msg_t *)calloc(1, sizeof (lccli_output_msg_t) + data.size());
       if(!out_msg) {
	     tracePrintf (TRACE_LEVEL_INFO, "lccli_cb_on_main_q Low memory\n");
	     *out = NULL;
	     return (-1);
       }
	   strcpy((char *)out_msg + sizeof (lccli_output_msg_t), data.c_str());
       out_msg->out_buf_len = data.size();
       break;

	default :
	  tracePrintf (TRACE_LEVEL_ERROR, "lccli_cb_handler: not upported\n");
	  out_msg = NULL;
	  break;
    }
    *out = out_msg;
    return (0);
}


// Setup Cstyle code to receive CPP msg
int setLccliMsgHandlerForClient()
{
     int status = 1 ;
     WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient (LccliMsgInstantiator);
     add_client_message_handler(LCCLI_MSG_CODE, lccli_cb_handler);

     return (status);
}

void setLccliManagementInterfaceReceiverPort (int serverPort)
{
   FrameworkToolKit::setManagementInterfaceReceiverPort (serverPort);
}

}
