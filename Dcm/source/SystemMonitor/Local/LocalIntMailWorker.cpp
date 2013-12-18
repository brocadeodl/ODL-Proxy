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
 *   Author : pahuja                                                       *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

#include "SystemMonitor/Global/IntMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/IntMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/LocalIntMailWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalIntMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalIntMailSetCfgMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

#include "DcmResourceIdEnums.h"
#include "thresh/fwdPub.h"

namespace DcmNs
{

    LocalIntMailWorker::LocalIntMailWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        addOperationMap (SYSTEMMONITORLOCALINTMAILSETEMAIL, reinterpret_cast<PrismMessageHandler> (&LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALINTMAILSETCFG, reinterpret_cast<PrismMessageHandler> (&LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler));
    }

    LocalIntMailWorker::~LocalIntMailWorker ()
    {
    }

    PrismMessage  *LocalIntMailWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALINTMAILSETEMAIL :
                pPrismMessage = new SystemMonitorLocalIntMailSetEmailMessage ();
                break;
            case SYSTEMMONITORLOCALINTMAILSETCFG :
                pPrismMessage = new SystemMonitorLocalIntMailSetCfgMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalIntMailWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId LocalIntMailWorker::pushBackend()
    {
        int status = 0;
        string emaillist;
        char errStr[150];

        // push the email list to backend
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(IntMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((pResults) && (pResults->size ()))
        { // accumulate list into string
            for (unsigned int i = 0; i < pResults->size(); i++)
            {
                IntMailListGlobalManagedObject *pMO = dynamic_cast<IntMailListGlobalManagedObject *> ((*pResults)[i]);
                string email = pMO->getEmail();
                // add token between email addresses in list
                if (i)
                {
                    emaillist += FWEMAIL_TOKEN_SEPERATOR;
                }
                emaillist += email;
                delete (*pResults)[i];
            }
        }
        // clean up search
        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;

        if (0 == emaillist.length())
        {
            emaillist = "none";
        }
        char *p_emaillist = (char *)emaillist.c_str();
        status = fwSetMailAddr(AGENT_GEPORT, p_emaillist, errStr);
        if (0 != status)
        {
            trace (TRACE_LEVEL_ERROR, string("LocalIntMailWorker::pushBackend email list error ") + string(errStr));
            status = 0;
        }


        // push the email enable flag to backend
        WaveManagedObjectSynchronousQueryContext *syncQuery2 = new WaveManagedObjectSynchronousQueryContext(IntMailCfgGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults2 = querySynchronously (syncQuery2);
        if ((NULL == pResults2) || (1 < pResults2->size ()))
        {   // error! cannot be more than one record
            trace (TRACE_LEVEL_ERROR, "LocalIntMailWorker::pushBackend email enable flag DB error");
        }
        else if (1 == pResults2->size ())
        {
            IntMailCfgGlobalManagedObject *pMO = dynamic_cast<IntMailCfgGlobalManagedObject *> ((*pResults2)[0]);
            bool enableFlag = pMO->getEnableFlag();
            if (true == enableFlag)
            {
                status = fwMailEnable(AGENT_GEPORT);
            }
            else
            {
                status = fwMailDisable(AGENT_GEPORT);
            }
        }
        else if (0 == pResults2->size ())
        {   // default false
            status = fwMailDisable(AGENT_GEPORT);
        }
        // clean up search2
        if (NULL != pResults2)
        {
            delete pResults2;
        }
        delete syncQuery2;
        if (0 != status)
        {
            trace (TRACE_LEVEL_ERROR, "LocalIntMailWorker::pushBackend email enable flag status" + FrameworkToolKit::localize (SYSTEMMONITOR_BACKEND_ERROR));
            status = 0;
        }

        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::pushBackend exiting");
        return WAVE_MESSAGE_SUCCESS; // return success so boot completes
    }

    void LocalIntMailWorker::boot (WaveAsynchronousContextForBootPhases
*pWaveAsynchronousContextForBootPhases)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason();

        if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason)
        { // only CONFIGURE needs to push data to backend.
            status = pushBackend();
        }

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

    int LocalIntMailWorker::fwBackendOpIntMail ()
    {
        if (WAVE_MESSAGE_SUCCESS == pushBackend())
        {
            return 0;
        }

        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::fwBackendOpIntMail err");
        return 1;
    }


    void  LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler( SystemMonitorLocalIntMailSetCfgMessage *pSystemMonitorLocalIntMailSetCfgMessage)
    {
        if (NULL == pSystemMonitorLocalIntMailSetCfgMessage)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalIntMailSetCfgMessageHandler: empty message");
            prismAssert (false, __FILE__, __LINE__);
        }

        ResourceId ret_status = WAVE_MESSAGE_SUCCESS;
        int status = -1;
        bool flag = false;

        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler Entering.");

        flag = pSystemMonitorLocalIntMailSetCfgMessage->getEnableFlag();
        if (true == flag)
        {
            status = fwMailEnable(AGENT_GEPORT); /* defined in fwdPub.h */
        }
        else
        {
            status = fwMailDisable(AGENT_GEPORT); /* defined in fwdPub.h */
        }

        trace (TRACE_LEVEL_INFO, string("LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler status is ") +status);
        switch (status)
        {
            case thUninitialized:
                 ret_status = SYSTEMMONITOR_FW_UNINIT_ERROR;
                 break;
            case thInvalidClass:
                 /* class is hardcoded to AGENT_GEPORT */
                 /* more likely error due to enable with no email addrs */
                 ret_status = SYSTEMMONITOR_MAILFLAG_ERROR;
                 /* ret_status = SYSTEMMONITOR_INVALID_CLASS; */
                 break;
            case thUnlicensed:
                 ret_status = SYSTEMMONITOR_NOT_LICENSED;
                 break;
            case 0:
                 ret_status = WAVE_MESSAGE_SUCCESS;
                 break;
            default:
                 ret_status = SYSTEMMONITOR_BACKEND_ERROR;
                 break;
        }

        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler status is " + FrameworkToolKit::localize(ret_status));
        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::SystemMonitorLocalIntMailSetCfgMessageHandler Exiting.");
        pSystemMonitorLocalIntMailSetCfgMessage->setCompletionStatus(ret_status);
        reply(pSystemMonitorLocalIntMailSetCfgMessage);
    }

    void  LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler( SystemMonitorLocalIntMailSetEmailMessage *pSystemMonitorLocalIntMailSetEmailMessage)
    {
        if (NULL == pSystemMonitorLocalIntMailSetEmailMessage)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalIntMailSetEmailMessageHandler: empty message");
            prismAssert (false, __FILE__, __LINE__);
        }

        ResourceId ret_status = WAVE_MESSAGE_SUCCESS;
        int status = -1;
        char errStr[150];

        string emails = pSystemMonitorLocalIntMailSetEmailMessage->getEmail();
        if (0 == emails.length())
        {
            emails = "none";
        }
        trace (TRACE_LEVEL_INFO, string("LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler setting email list to ") + emails);
        char *p_emaillist = (char *)emails.c_str();
        status = fwSetMailAddr(AGENT_GEPORT, p_emaillist, errStr);
        trace (TRACE_LEVEL_INFO, string("LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler status is ") +status);

        switch (status)
        {
            case thUninitializedFWD:
                 ret_status = SYSTEMMONITOR_FW_UNINIT_ERROR;
                 break;
            case thInvalidClass:
                 ret_status = SYSTEMMONITOR_INVALID_CLASS;
                 break;
            case thUnlicensed:
                 ret_status = SYSTEMMONITOR_NOT_LICENSED;
                 break;
            case 0:
                 ret_status = WAVE_MESSAGE_SUCCESS;
                 break;
            default:
                 ret_status = SYSTEMMONITOR_EMAIL_ERROR;
                 trace (TRACE_LEVEL_INFO, string("LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler status is ") + string(errStr));
                 break;
        }

        trace (TRACE_LEVEL_INFO, "LocalIntMailWorker::SystemMonitorLocalIntMailSetEmailMessageHandler Exiting.");
        pSystemMonitorLocalIntMailSetEmailMessage->setCompletionStatus(ret_status);
        reply(pSystemMonitorLocalIntMailSetEmailMessage);

    }

}
