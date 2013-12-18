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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "DcmResourceIds.h"
#include "DcmCore/DcmToolKit.h"


#include "SystemMonitor/Local/LocalRelayIpWorker.h"
#include "SystemMonitor/Local/RelayIpLocalConfigMessage.h"
#include "SystemMonitor/Local/RelayIpLocalSetMessage.h"
#include "SystemMonitor/Local/RelayIpLocalDelMessage.h"
//#include "SystemMonitor/Local/RelayIpLocalConfigMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"
#include "SystemMonitor/Global/RelayIpManagedObject.h"
#define PATTERN_STRING "#System_Monitor_Mail"
#define COMMAND_SIZE 1024
namespace DcmNs
{

    LocalRelayIpWorker::LocalRelayIpWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        addOperationMap (RELAYIPLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&LocalRelayIpWorker::RelayIpLocalConfigMessageHandler));
        addOperationMap (RELAYIPLOCALSET, reinterpret_cast<PrismMessageHandler> (&LocalRelayIpWorker::RelayIpLocalSetMessageHandler));
        addOperationMap (RELAYIPLOCALDEL, reinterpret_cast<PrismMessageHandler> (&LocalRelayIpWorker::RelayIpLocalDelMessageHandler));
    }

    LocalRelayIpWorker::~LocalRelayIpWorker ()
    {
    }

    PrismMessage  *LocalRelayIpWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case RELAYIPLOCALCONFIG :
                pPrismMessage = new RelayIpLocalConfigMessage ();
                break;
			case RELAYIPLOCALSET :
                pPrismMessage = new RelayIpLocalSetMessage ();
                break;
            case RELAYIPLOCALDEL :
                pPrismMessage = new RelayIpLocalDelMessage ();
                break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
	
	void  LocalRelayIpWorker::fwBackendRelay () {
		char command[1024];

        RelayIpManagedObject *pRelayIpManagedObject = NULL;

		WaveManagedObjectSynchronousQueryContext *syncQuery = new
                WaveManagedObjectSynchronousQueryContext(RelayIpManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
		int i = 0;

        if (pResults->size() == 0) {
			i = system ("/bin/sed -e \"s/^DS.*/DS/\" < /etc/mail/submit.cf > /etc/mail/submit.tmp;/bin/mv /etc/mail/submit.tmp /etc/mail/submit.cf");
			snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts",PATTERN_STRING);
			i = system (command);
		} else {
			pRelayIpManagedObject = dynamic_cast <RelayIpManagedObject *> ((*pResults)[0]);
			HostUC ipAddr = pRelayIpManagedObject->getHostip();
			string ipVal  = ipAddr.getHostValue();
			string domainStr = pRelayIpManagedObject->getDomainname();
			memset (command, '\0', sizeof (command));
			snprintf (command, sizeof(command), "/bin/sed -e \"s/^DS.*/DS[%s]/\" < /etc/mail/submit.cf > /etc/mail/submit.tmp;/bin/mv /etc/mail/submit.tmp /etc/mail/submit.cf", ipVal.c_str());
			i = system (command);
			memset (command, '\0', sizeof (command));
			snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;echo %s\t\t%s\t\t'%s'>> /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts", PATTERN_STRING, ipVal.c_str(), domainStr.c_str(), PATTERN_STRING);
			i = system (command);

		}
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/mail/submit.cf");
		 i = system (command);
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/hosts");
		 i = system (command);
        trace (TRACE_LEVEL_DEBUG, string ("LocalRelayIpWorker::fwBackendRelay i=")+i);
		delete pRelayIpManagedObject;
		delete pResults;
		delete syncQuery;
	}

	void  LocalRelayIpWorker::RelayIpLocalSetMessageHandler( RelayIpLocalSetMessage *pRelayIpLocalSetMessage)
    {
        trace (TRACE_LEVEL_INFO, "LocalRelayIpWorker::RelayIpLocalSetMessageHandler:Entering ");

		if (pRelayIpLocalSetMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Contact:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		HostUC ipAdrr		   = pRelayIpLocalSetMessage->getHostip();
		string domainStr	   = pRelayIpLocalSetMessage->getDomainname();
		string ipVal		   = ipAdrr.getHostValue();
		char command[COMMAND_SIZE];
		int i = 0;
		memset (command, '\0', sizeof (command));
		snprintf (command, sizeof(command), "/bin/sed -e \"s/^DS.*/DS[%s]/\" < /etc/mail/submit.cf > /etc/mail/submit.tmp;/bin/mv /etc/mail/submit.tmp /etc/mail/submit.cf", ipVal.c_str());
		i = system (command);
		if (domainStr.length () != 0) {
			memset (command, '\0', sizeof (command));
			snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;echo %s\t\t%s\t\t'%s'>> /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts", PATTERN_STRING, ipVal.c_str(), domainStr.c_str(), PATTERN_STRING);
			i = system (command);
		} else {
			memset (command, '\0', sizeof (command));
			snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts",PATTERN_STRING);
			i = system (command);
		}
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/mail/submit.cf");
		 i = system (command);
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/hosts");
		 i = system (command);
        trace (TRACE_LEVEL_DEBUG, string ("LocalRelayIpWorker::RelayIpLocalSetMessageHandler i=")+i);
		pRelayIpLocalSetMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pRelayIpLocalSetMessage);

    }

    void  LocalRelayIpWorker::RelayIpLocalDelMessageHandler( RelayIpLocalDelMessage *pRelayIpLocalDelMessage)
    {
        trace (TRACE_LEVEL_INFO, "System Group Worker::Set RelayIp Msg Handler:Entering ");

		if (pRelayIpLocalDelMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Contact:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		HostUC ipAddr		   = pRelayIpLocalDelMessage->getHostip();
		string ipVal		   = ipAddr.getHostValue();
		char command[COMMAND_SIZE];
		int i = 0;
		memset (command, '\0', sizeof (command));
		i = system ("/bin/sed -e \"s/^DS.*/DS/\" < /etc/mail/submit.cf > /etc/mail/submit.tmp;/bin/mv /etc/mail/submit.tmp /etc/mail/submit.cf");
		snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts",PATTERN_STRING);
		i = system (command);
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/mail/submit.cf");
		 i = system (command);
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/hosts");
		 i = system (command);
        trace (TRACE_LEVEL_DEBUG, string ("LocalRelayIpWorker::RelayIpLocalDelMessageHandler i=")+i);
		pRelayIpLocalDelMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pRelayIpLocalDelMessage);

    }

    void  LocalRelayIpWorker::RelayIpLocalConfigMessageHandler( RelayIpLocalConfigMessage *pRelayIpLocalConfigMessage)
    {
        trace (TRACE_LEVEL_INFO, "LocalRelayIpWorker::RelayIpLocalConfigMessageHandler:Entering ");

		if (pRelayIpLocalConfigMessage == NULL) {
            trace (TRACE_LEVEL_FATAL, "Set Contact:NULL message arrived");
            prismAssert (false, __FILE__, __LINE__);
		}
		HostUC ipAdrr		   = pRelayIpLocalConfigMessage->getHostip();
		string domainStr	   = pRelayIpLocalConfigMessage->getDomainname();
		string ipVal		   = ipAdrr.getHostValue();
		char command[COMMAND_SIZE];
		int i = 0;
		memset (command, '\0', sizeof (command));
		snprintf (command, sizeof(command), "/bin/sed -e \"s/^DS.*/DS[%s]/\" < /etc/mail/submit.cf > /etc/mail/submit.tmp;/bin/mv /etc/mail/submit.tmp /etc/mail/submit.cf", ipVal.c_str());
		i = system (command);
		if (domainStr.length () != 0) {
			memset (command, '\0', sizeof (command));
			snprintf (command, sizeof(command), "/bin/grep -v '%s' /etc/hosts > /etc/hosts.temp;echo %s\t\t%s\t\t'%s'>> /etc/hosts.temp;/bin/mv /etc/hosts.temp /etc/hosts", PATTERN_STRING, ipVal.c_str(), domainStr.c_str(), PATTERN_STRING);
			i = system (command);
		}
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/mail/submit.cf");
		 i = system (command);
		 memset (command, '\0', sizeof (command));
		 snprintf (command, sizeof(command), "/fabos/cliexec/config save /etc/hosts");
		 i = system (command);
        trace (TRACE_LEVEL_DEBUG, string ("LocalRelayIpWorker::RelayIpLocalConfigMessageHandler i=")+i);
		pRelayIpLocalConfigMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        reply (pRelayIpLocalConfigMessage);

    }
}
