#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "Framework/Utils/FrameworkToolKit.h"

#include "TrillOam/Common/TrillOamPluginTypes.h"
#include "ClientInterface/TrillOam/TrillOamMangConfigHandler.h"
#include "ValClientInterface/DcmClientInterface.h"




int trilloam_ping_handler (const void *inp, void ** const out)
{
	printf("%s \n", __FUNCTION__);
		trilloam_msg_ping *config = (trilloam_msg_ping *) inp;
        printf( "        m_cmdCode = %d \n", config->cmd );
        printf( "        m_src_mac = %s \n", config->src_mac);
        printf( "        m_dst_mac = %s \n", config->dst_mac);
        printf( "        m_vlanid = %d \n", config->vlanid );
	return 0;
}

int trilloam_traceroute_handler (const void *inp, void ** const out)
{
	static int i =0;
//trilloam_msg_traceroute_resp *resp_p = NULL;
//int size = sizeof(trilloam_msg_traceroute_resp);
//resp_p = (trilloam_msg_traceroute_resp *) calloc(1, size);
//memset(resp_p, 0, size);
//*out = resp_p;

	printf("%s \n", __FUNCTION__);
		trilloam_msg_traceroute *config = (trilloam_msg_traceroute *) inp;
        printf( "        m_cmdCode = %d \n", config->cmd );
        printf( "        m_src_mac = %s \n", config->src_mac );
        printf( "        m_dst_mac = %s \n", config->dst_mac );
        printf( "        m_vlanid = %d \n", config->vlanid );
        printf( "        m_src_ip = %s \n", config->src_ip );
        printf( "        m_dst_ip = %s \n", config->dst_ip );
        printf( "        m_protocol = %s \n", config->proto );
        printf( "        m_src_port = %d \n", config->src_port );
        printf( "        m_dst_port = %d \n", config->dst_port );
		config->last = 0;
		i++;
		if (i> 3)
			config->last =1;
		printf("        last is%d \n", config->last);


	return 0;
}

int main()
{
    int ret = 0;

    /* Set DCM client hander  */
    setTrillOamMangConfigHandlerForClient();

    /* Set MSTP cmd callback function */
    add_client_message_handler(TRILLOAMPING, trilloam_ping_handler);
    add_client_message_handler(TRILLOAMTRACEROUTE, trilloam_traceroute_handler);
	    
	//WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(9122);
	 WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(9119);

    /* Initialize Dcmd */
    ret = dcm_initialize_client("trilloam");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from TRILLOAM client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from TRILLOAM client... \n");
    } 

    while(1)
        sleep(1000000);

	return ret;
}
