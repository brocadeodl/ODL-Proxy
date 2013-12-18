#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "ClientInterface/Lccli/LccliClient.h"
#include "ValClientInterface/DcmClientInterface.h"
#include <assert.h>
#include "ClientInterface/DceWaveClientMessage.h"
#include "DcmCStatus.h"
#include "WaveCStatus.h"


static int
lccli_cb_handler (const void *inp, void ** const out)a
{
	lccli_input_msg_t *in_msg = (lccli_input_msg_t *) inp;

	printf("lccli_cb_handler cmd %s code %d\n", in_msg->argv ,
					in_msg->cli_code);
	return (0);
}

int dummy(const void *inp, void ** const out)
{
    return WRC_WAVE_MESSAGE_SUCCESS;
}

int main(int argc, char **argv)
{
    int ret = 0;

    int mgmt_port = 9109;

    if (argc != 2) {
    	printf("usage: lccli_app.Linux.i686 mgmt_port\n");
    	return 0;
	}

	mgmt_port += atoi(argv[1]);


    /* STEP 2 */
    setLccliMsgHandlerForClient();

    /* STEP 3 */
	add_client_message_handler(LCCLI_MSG_CODE, lccli_cb_handler);

	WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);


    /* STEP 4: Initialize Dcmd */
    ret = dcm_initialize_client (LCCLI_CLIENT_NAME);
    if(ret != 0 )
    {
		printf("dcm init fail for lccli client %d\n", ret);
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from lccli client... \n");
    } 

    while(1)
        sleep(100);
}
