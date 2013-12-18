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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#include <string.h>
#include <time.h>
#include <utmp.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Types/IpVxAddress.h"
#include "WyserEaGateway/WyserUserInfo.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"

#include "AAA/Local/AAALocalLocalObjectManager.h"
#include "AAA/Local/LocalSessionWorker.h"
#include "AAA/Local/AAALocalGetActiveUsersMessage.h"
#include "AAA/Local/AAALocalGetLockedUsersMessage.h"
#include "AAA/Local/AAALocalTypes.h"

extern "C"
{
#include "security/public.h"
}


namespace DcmNs
{

    LocalSessionWorker::LocalSessionWorker ( AAALocalLocalObjectManager *pAAALocalLocalObjectManager)
        : WaveWorker  (pAAALocalLocalObjectManager)
    {
        addOperationMap (AAALOCALGETACTIVEUSERS, reinterpret_cast<PrismMessageHandler> (&LocalSessionWorker::AAALocalGetActiveUsersMessageHandler));
        addOperationMap (AAALOCALGETLOCKEDUSERS, reinterpret_cast<PrismMessageHandler> (&LocalSessionWorker::AAALocalGetLockedUsersMessageHandler));
    }

    LocalSessionWorker::~LocalSessionWorker ()
    {
    }

    PrismMessage  *LocalSessionWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case AAALOCALGETACTIVEUSERS :
                pPrismMessage = new AAALocalGetActiveUsersMessage ();
                break;
            case AAALOCALGETLOCKEDUSERS :
                pPrismMessage = new AAALocalGetLockedUsersMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalSessionWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  LocalSessionWorker::AAALocalGetLockedUsersMessageHandler( AAALocalGetLockedUsersMessage *pAAALocalGetLockedUsersMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetLockedUsersMessageHandler Enter."));

        // Validations and Queries should go here
        if (pAAALocalGetLockedUsersMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        pAAALocalGetLockedUsersMessage->clearNumVectors();
        // get locked users and loop through records
        int i, nacct = 0, ret = 0;
        secAcct_t *pacct = NULL;

        ret = secAcctGet(NULL, &pacct, &nacct, 0);
        if (ret == SEC_OK)
        {
            if (NULL != pacct)
            {
                for (i = 0; i < nacct; i++)
                {
                    if (1 == pacct[i].locked)
                    {
                        pAAALocalGetLockedUsersMessage->setLockedUser(string(pacct[i].name));
                    }
                }
                free(pacct);
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string("LocalSessionWorker::AAALocalGetLockedUsersMessageHandler secAcctGet returned ") + ret);
            switch (ret)
            {
                case SEC_NO_MEMORY:
                    status = SHOWUSER_NO_MEM;
                    break;
                case SEC_NO_ACCESS:
                    status = SHOWUSER_NO_ACCESS;
                    break;
                case ACCT_DB_ERROR:
                    status = SHOWUSER_DB_ERROR;
                    break;
                default:
                    status = SHOWUSER_BACKEND_ERROR;
                    break;
            }
            trace (TRACE_LEVEL_ERROR, string("LocalSessionWorker::AAALocalGetLockedUsersMessageHandler ") + FrameworkToolKit::localize (status));
        }

        trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetLockedUsersMessageHandler Exiting."));
        pAAALocalGetLockedUsersMessage->setCompletionStatus(status);
        reply (pAAALocalGetLockedUsersMessage);
    }

    void  LocalSessionWorker::AAALocalGetActiveUsersMessageHandler( AAALocalGetActiveUsersMessage *pAAALocalGetActiveUsersMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUsersMessagHandler Enter."));

        // Validations and Queries should go here
        if (pAAALocalGetActiveUsersMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        pAAALocalGetActiveUsersMessage->clearNumVectors();

		/* Get admin and user defined users */
		vector<WyserUserInfo *>currentCliSessions = DcmToolKit::getCurrentCliSessions();
		UI32 num_cli_sessions = currentCliSessions.size();

		trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUserMessageHandler number of Cli sessions is ") + num_cli_sessions);
		for (UI32 idx = 0; idx < num_cli_sessions; idx++)
		{
			string username = currentCliSessions[idx]->getUsername();
			string hostname = "";
			UI32 session_id = currentCliSessions[idx]->getSessionId();
			IpVxAddress ipvxAddr = currentCliSessions[idx]->getIpAddress();
			string devicename = "Cli";
			UI32 pid = 0;
			DateTime loginTime = currentCliSessions[idx]->getLoginDateTime();
			string role =  currentCliSessions[idx]->getRole();

			pAAALocalGetActiveUsersMessage->setActiveUser(username, hostname, session_id, ipvxAddr.toString(), devicename, pid, loginTime, role);
			trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUserMessageHandler Cli user ") + username + ", session " + session_id + ", ip addr " + ipvxAddr.toString() + ", timestamp " + loginTime.toString() + role);
		}
		currentCliSessions.clear();

        // use DcmToolKit for netconf sessions (getutent does not get)
        vector<WyserUserInfo *>currentNetconfSessions = DcmToolKit::getCurrentNetconfSessions();
        UI32 num_sessions = currentNetconfSessions.size();

        trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUserMessageHandler number of netconf sessions is ") + num_sessions);
        for (UI32 idx = 0; idx < num_sessions; idx++)
        {
            string username = currentNetconfSessions[idx]->getUsername();
            string hostname = "";
            UI32 session_id = currentNetconfSessions[idx]->getSessionId();
            IpVxAddress ipvxAddr = currentNetconfSessions[idx]->getIpAddress();
            string devicename = "netconf";
            UI32 pid = 0;
            DateTime loginTime = currentNetconfSessions[idx]->getLoginDateTime();
			string role =  currentNetconfSessions[idx]->getRole();

            pAAALocalGetActiveUsersMessage->setActiveUser(username, hostname, session_id, ipvxAddr.toString(), devicename, pid, loginTime, role);

            trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUserMessageHandler netconf user ") + username + ", session " + session_id + ", ip addr " + ipvxAddr.toString() + ", timestamp " + loginTime.toString() + string(", Role ") + role);
        }
        currentNetconfSessions.clear();

        trace (TRACE_LEVEL_INFO, string("LocalSessionWorker::AAALocalGetActiveUsersMessagHandler Exiting."));
        pAAALocalGetActiveUsersMessage->setCompletionStatus(status);
        reply (pAAALocalGetActiveUsersMessage);
    }

}
