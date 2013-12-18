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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/PamUtils.h"

#include <stdlib.h>

extern "C"
{

#include <security/pam_appl.h>
#include <security/pam_misc.h>

int pamConversation (int numberOfMessages, const struct pam_message **pMessage, struct pam_response **pResponse, void *pApplicationData)
{
    int                  i         = 0;
    struct pam_response *pReply    = (struct pam_response *) calloc (numberOfMessages, sizeof (struct pam_response));
    char                *pPassword = (char *) malloc (512);

    //printf ("Number of messages : %d\n", numberOfMessages);

    for (i = 0; i < numberOfMessages; i++)
    {
        //printf ("Style : %d\n", pMessage[i]->msg_style);
        //printf ("%s\n", pMessage[i]->msg);
    }

    strncpy (pPassword, (char *) pApplicationData, 512);

    pReply[0].resp_retcode = 0;
    pReply[0].resp = (char *) pPassword;

    //printf ("Password :\"%s\"\n", pPassword);

    *pResponse = pReply;

    return (PAM_SUCCESS);
}

}

namespace WaveNs
{

bool PamUtils::authenticate (const string &authenticationService, const string& userName, const string &password)
{
    char                  *pApplicationData     = (char *) malloc (512);
    const struct pam_conv  localConversation    = {&::pamConversation, pApplicationData};
    pam_handle_t          *pPamHandle           = NULL;
    int                    localStatus          = 0;
    int                    authenticationStatus = 0;

    strncpy (pApplicationData, password.c_str (), 512);

    localStatus = pam_start (authenticationService.c_str (), userName.c_str (), &localConversation, &pPamHandle);

//printf ("Status 2: %d\n", localStatus);

    authenticationStatus = pam_authenticate (pPamHandle , PAM_SILENT);
//printf ("Status : %d\n", authenticationStatus);

    localStatus = pam_end (pPamHandle , localStatus);
//printf ("Status : %d\n", localStatus);

    if (PAM_SUCCESS == authenticationStatus)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

}
