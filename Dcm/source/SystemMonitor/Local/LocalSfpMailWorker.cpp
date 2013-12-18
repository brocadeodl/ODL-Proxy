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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

#include "SystemMonitor/Global/SfpMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/SfpMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/LocalSfpMailWorker.h"
#include "SystemMonitor/Local/SystemMonitorLocalSfpMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalSfpMailSetCfgMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

#include "DcmResourceIdEnums.h"
#include "thresh/fwdPub.h"

namespace DcmNs
{

    LocalSfpMailWorker::LocalSfpMailWorker ( SystemMonitorLocalObjectManager *pSystemMonitorLocalObjectManager)
        : WaveWorker  (pSystemMonitorLocalObjectManager)
    {
        addOperationMap (SYSTEMMONITORLOCALSFPMAILSETEMAIL, reinterpret_cast<PrismMessageHandler> (&LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler));
        addOperationMap (SYSTEMMONITORLOCALSFPMAILSETCFG, reinterpret_cast<PrismMessageHandler> (&LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler));
    }

    LocalSfpMailWorker::~LocalSfpMailWorker ()
    {
    }

    PrismMessage  *LocalSfpMailWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORLOCALSFPMAILSETEMAIL :
                pPrismMessage = new SystemMonitorLocalSfpMailSetEmailMessage ();
                break;
            case SYSTEMMONITORLOCALSFPMAILSETCFG :
                pPrismMessage = new SystemMonitorLocalSfpMailSetCfgMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *LocalSfpMailWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId LocalSfpMailWorker::pushBackend()
    {
        int status = 0;
        string emaillist;
        char errStr[150];

        // push the email list to backend
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(SfpMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((pResults) && (pResults->size ()))
        { // accumulate list into string
            for (unsigned int i = 0; i < pResults->size(); i++)
            {
                SfpMailListGlobalManagedObject *pMO = dynamic_cast<SfpMailListGlobalManagedObject *> ((*pResults)[i]);
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
        status = fwSetMailAddr(AGENT_GBIC, p_emaillist, errStr);
        if (0 != status)
        {
            trace (TRACE_LEVEL_ERROR, string("LocalSfpMailWorker::pushBackend email list error ") + string(errStr));
            status = 0;
        }


        // push the email enable flag to backend
        WaveManagedObjectSynchronousQueryContext *syncQuery2 = new WaveManagedObjectSynchronousQueryContext(SfpMailCfgGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults2 = querySynchronously (syncQuery2);
        if ((NULL == pResults2) || (1 < pResults2->size ()))
        {   // error! cannot be more than one record
            trace (TRACE_LEVEL_ERROR, "LocalSfpMailWorker::pushBackend email enable flag DB error");
        }
        else if (1 == pResults2->size ())
        {
            SfpMailCfgGlobalManagedObject *pMO = dynamic_cast<SfpMailCfgGlobalManagedObject *> ((*pResults2)[0]);
            bool enableFlag = pMO->getEnableFlag();
            if (true == enableFlag)
            {
                status = fwMailEnable(AGENT_GBIC);
            }
            else
            {
                status = fwMailDisable(AGENT_GBIC);
            }
        }
        else if (0 == pResults2->size ())
        {   // default false
            status = fwMailDisable(AGENT_GBIC);
        }
        // clean up search2
        if (NULL != pResults2)
        {
            delete pResults2;
        }
        delete syncQuery2;
        if (0 != status)
        {
            trace (TRACE_LEVEL_ERROR, "LocalSfpMailWorker::pushBackend email enable flag status" + FrameworkToolKit::localize (SYSTEMMONITOR_BACKEND_ERROR));
            status = 0;
        }

        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::pushBackend exiting");
        return WAVE_MESSAGE_SUCCESS; // return success so boot completes
    }

    void LocalSfpMailWorker::boot (WaveAsynchronousContextForBootPhases
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

    int LocalSfpMailWorker::fwBackendOpSfpMail ()
    {
        if (WAVE_MESSAGE_SUCCESS == pushBackend())
        {
            return 0;
        }

        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::fwBackendOpSfpMail err");
        return 1;
    }


    void  LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler( SystemMonitorLocalSfpMailSetCfgMessage *pSystemMonitorLocalSfpMailSetCfgMessage)
    {
        if (NULL == pSystemMonitorLocalSfpMailSetCfgMessage)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalSfpMailSetCfgMessageHandler: empty message");
            prismAssert (false, __FILE__, __LINE__);
        }

        ResourceId ret_status = WAVE_MESSAGE_SUCCESS;
        int status = -1;
        bool flag = false;

        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler Entering.");

        flag = pSystemMonitorLocalSfpMailSetCfgMessage->getEnableFlag();
        if (true == flag)
        {
            status = fwMailEnable(AGENT_GBIC); /* defined in fwdPub.h */
        }
        else
        {
            status = fwMailDisable(AGENT_GBIC); /* defined in fwdPub.h */
        }

        trace (TRACE_LEVEL_INFO, string("LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler status is ") +status);
        switch (status)
        {
            case thUninitialized:
                 ret_status = SYSTEMMONITOR_FW_UNINIT_ERROR;
                 break;
            case thInvalidClass:
                 /* class is hardcoded to AGENT_GBIC */
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

        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler status is " + FrameworkToolKit::localize(ret_status));
        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::SystemMonitorLocalSfpMailSetCfgMessageHandler Exiting.");
        pSystemMonitorLocalSfpMailSetCfgMessage->setCompletionStatus(ret_status);
        reply(pSystemMonitorLocalSfpMailSetCfgMessage);
    }

    void  LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler( SystemMonitorLocalSfpMailSetEmailMessage *pSystemMonitorLocalSfpMailSetEmailMessage)
    {
        if (NULL == pSystemMonitorLocalSfpMailSetEmailMessage)
        {
            trace (TRACE_LEVEL_INFO, "SystemMonitorLocalSfpMailSetEmailMessageHandler: empty message");
            prismAssert (false, __FILE__, __LINE__);
        }

        ResourceId ret_status = WAVE_MESSAGE_SUCCESS;
        int status = -1;
        char errStr[150];

        string emails = pSystemMonitorLocalSfpMailSetEmailMessage->getEmail();
        if (0 == emails.length())
        {
            emails = "none";
        }
        trace (TRACE_LEVEL_INFO, string("LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler setting email list to ") + emails);
        char *p_emaillist = (char *)emails.c_str();
        status = fwSetMailAddr(AGENT_GBIC, p_emaillist, errStr);
        trace (TRACE_LEVEL_INFO, string("LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler status is ") +status);

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
                 trace (TRACE_LEVEL_INFO, string("LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler status is ") + string(errStr));
                 break;
        }

        trace (TRACE_LEVEL_INFO, "LocalSfpMailWorker::SystemMonitorLocalSfpMailSetEmailMessageHandler Exiting.");
        pSystemMonitorLocalSfpMailSetEmailMessage->setCompletionStatus(ret_status);
        reply(pSystemMonitorLocalSfpMailSetEmailMessage);

    }

}
