/***************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen                                    *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "DcmCore/DcmToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/RAS/RASClient.h"

#include <raslog/raslogm.h>
#include <utils/node_id.h>

using namespace WaveNs;
using namespace DcmNs;

#define UI32	int
#define SI32	int

#define  RASLOG_LOG printf

/* C style callback funtion to receive DCM msg */
int printRASLogMsg(const void *inp, void ** const out)
{
    vcslog_t *vcslog = (vcslog_t *)inp;

    if (vcslog == NULL)
        return (-1);

    printf("Name :\n");
    *out = NULL;

    return (0);
}

int
main(int argc, char *argv[])
{
    UI32    serverPort                   = 9110;
    UI32    numberOfTimesToExecuteTest   = 0;
    UI32    i                            = 0;
    UI32    numberOfCommandLineArguments = (UI32) argc;
    SI32    clientPort                   = 9810;
    SI32    rc = 0;
    vcslog_t vcslog;
    char test[24] = "test";
    SI32    delay = 2;

    for (i = 1; i < numberOfCommandLineArguments; i++)
    {
        if (0 == strcasecmp ("-serverport", argv[i]))
        {
            serverPort = atoi (argv[i + 1]);
	    setManagementInterfaceReceiverPort (serverPort);
            i++;

        } else if (0 == strcasecmp ("-loop", argv[i])) {
            numberOfTimesToExecuteTest = atoi (argv[i + 1]);

            i++;

        } else if (0 == strcasecmp ("-clientport", argv[i])) {
            clientPort = atoi (argv[i + 1]);
            i++;

        } else if (0 == strcasecmp ("-vcsmsg", argv[i])) {
            strcpy (test, argv[i + 1]);
            i++;

        } else if (0 == strcasecmp ("-delay", argv[i])) {
            delay = atoi (argv[i + 1]);
            i++;
        }  else if (0 == strcasecmp ("-instance", argv[i])) {
            thisSlotInstance = atoi (argv[i + 1]);
            i++;
	} else if (0 == strcasecmp ("-help", argv[i])) {
	    printf ("-serverport\n");
	    printf ("-clientport\n");
	    printf ("-vcsmsg\n");
	    printf ("-delay\n");
	    printf ("-loop\n");
            printf ("-instnace\n");
	    return (0);
            i++;

        } else {
	    printf ("Invalid CLI arguement\n");
	    printf ("-serverport\n");
	    printf ("-clientport\n");
	    printf ("-vcsmsg\n");
	    printf ("-delay\n");
	    printf ("-loop\n");
            printf ("-instnace\n");
	    return (0);
	}
    }
 
    RASLOG_LOG("RAS Dcm-client setRASMsgHandlerForClient");
    /* Set handler method*/ 
    setRASMsgHandlerForClient();

    /* 3. Initialize Dcmd */
    RASLOG_LOG("RAS Dcm-client dcm_initialize_client");
    if (dcm_initialize_client("RAS")) {
	    RASLOG_LOG("RAS Dcm-client Init failed");
	    return (-1);

    } else {
	    RASLOG_LOG("RAS Dcm-client Init okay");
    }

    if (numberOfTimesToExecuteTest != 0) {
        while(numberOfTimesToExecuteTest) {
	    sprintf((char *)&test, "%s%d", "test", numberOfTimesToExecuteTest);
	    printf("Send VCSMSG=%s\n", test);
	    RLS_SET_SWNAME(&vcslog.vcsmsg, test);
	    RLS_SET_VER(&vcslog.vcsmsg, 0x12);
	    RLS_SET_MAGIC(&vcslog.vcsmsg, 0x34);

	    if ((rc = send_vcslog (&vcslog)) < 0) {
	        printf ("Fail to send vcslog rc=%d\n", rc);
		return (0);
	    }
	    numberOfTimesToExecuteTest--;
	    sleep(delay);
	}
    }

    while(1)
        sleep(100);

    return (0);

}
