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

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalSfpMailWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalSfpMailSetCfgMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalSfpMailAddEmailMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalSfpMailRemoveEmailMessage.h"
#include "SystemMonitor/Global/SfpMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/SfpMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalSfpMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalSfpMailSetCfgMessage.h"

#include "DcmResourceIdEnums.h"
#include "thresh/fwdPub.h"

#include "AAA/Global/AAASequencerContexts.h"

namespace DcmNs
{

    GlobalSfpMailWorker::GlobalSfpMailWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        SfpMailCfgGlobalManagedObject    SfpMailCfgGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        SfpMailCfgGlobalManagedObject.setupOrm ();
        addManagedClass (SfpMailCfgGlobalManagedObject::getClassName (), this);
        SfpMailListGlobalManagedObject    SfpMailListGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        SfpMailListGlobalManagedObject.setupOrm ();
        addManagedClass (SfpMailListGlobalManagedObject::getClassName (), this);

        addOperationMap (SYSTEMMONITORGLOBALSFPMAILSETCFG, reinterpret_cast<PrismMessageHandler> (&GlobalSfpMailWorker::SystemMonitorGlobalSfpMailSetCfgMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALSFPMAILADDEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalSfpMailWorker::SystemMonitorGlobalSfpMailAddEmailMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALSFPMAILREMOVEEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalSfpMailWorker::SystemMonitorGlobalSfpMailRemoveEmailMessageHandler));
    }

    GlobalSfpMailWorker::~GlobalSfpMailWorker ()
    {
    }

    PrismMessage  *GlobalSfpMailWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALSFPMAILSETCFG :
                pPrismMessage = new SystemMonitorGlobalSfpMailSetCfgMessage ();
                break;
            case SYSTEMMONITORGLOBALSFPMAILADDEMAIL :
                pPrismMessage = new SystemMonitorGlobalSfpMailAddEmailMessage ();
                break;
            case SYSTEMMONITORGLOBALSFPMAILREMOVEEMAIL :
                pPrismMessage = new SystemMonitorGlobalSfpMailRemoveEmailMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalSfpMailWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((SfpMailCfgGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SfpMailCfgGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((SfpMailListGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new SfpMailListGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalSfpMailWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }

    void  GlobalSfpMailWorker::SystemMonitorGlobalSfpMailAddEmailMessageHandler( SystemMonitorGlobalSfpMailAddEmailMessage *pSystemMonitorGlobalSfpMailAddEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::SystemMonitorGlobalSfpMailAddEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalSfpMailAddEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL message in handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        email = pSystemMonitorGlobalSfpMailAddEmailMessage->getEmail();
        // get list of emails from managed object and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(SfpMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added email
            trace (TRACE_LEVEL_INFO, string("SystemMonitorGlobalSfpMailAddEmailMessageHandler: email list now has one entry"));
        }
        else if (MAX_SUPPORTED_MAIL_ADDRESS <= pResults->size ())
        {
            // already have max number of configured email addresses
            status = SYSTEMMONITOR_EMAIL_MAX;
            trace (TRACE_LEVEL_ERROR, FrameworkToolKit::localize (status) + email);
            added = false;
        }
        else
        {
            unsigned int i;

            // accumulate list into string, append new email and send to local
            for (i = 0; i < pResults->size(); i++)
            {
                SfpMailListGlobalManagedObject *pMO = dynamic_cast<SfpMailListGlobalManagedObject *> ((*pResults)[i]);
                string tempEmail = pMO->getEmail();
                if (tempEmail == email)
                {
                    // server is already in list.  set status
                    status = SYSTEMMONITOR_EMAIL_IN_LIST;
                    trace (TRACE_LEVEL_ERROR, email + FrameworkToolKit::localize (status));
                    added = false;
                    break;
                }
                // add token between email addresses in list
                if (i)
                {
                    emaillist += FWEMAIL_TOKEN_SEPERATOR;
                }
                emaillist += tempEmail;
                delete (*pResults)[i];
            }
            emaillist += FWEMAIL_TOKEN_SEPERATOR;
        }
        // append new server to end of list
        emaillist += email;

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;

        if (false == added)
        {
            // too many emails or email already in list.  return status
            pSystemMonitorGlobalSfpMailAddEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalSfpMailAddEmailMessage);
            return;
        }

        // send email list to cluster
        SystemMonitorLocalSfpMailSetEmailMessage *pMessage = new SystemMonitorLocalSfpMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalSfpMailWorker::addEmailSTCCallback), pSystemMonitorGlobalSfpMailAddEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalSfpMailWorker::addEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::addEmailSTCCallback Enter"));

        SystemMonitorGlobalSfpMailAddEmailMessage *pOrigMsg = (SystemMonitorGlobalSfpMailAddEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId ret_status = pWaveSendToClusterContext->getCompletionStatus();
		AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;
		bool allFailed = false;
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                ret_status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "Add email operation failed on one or more of the connected nodes.");
            else {
                trace (TRACE_LEVEL_ERROR, "Add email operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalSfpMailWorker::addEmailSTCCallback", errorMessage, true);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    ret_status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        {
            trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::addEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // add email to the managed object
            startTransaction();
            SfpMailListGlobalManagedObject *pMO = dynamic_cast<SfpMailListGlobalManagedObject *>(createManagedObjectInstance(SfpMailListGlobalManagedObject::getClassName()));
            pMO->setEmail(email);
            ret_status = commitTransaction();
            delete pMO;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::addEmailSTCCallback added email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalSfpMailWorker::addEmailSTCCallback: failed adding email to DB"));
                ret_status = SYSTEMMONITOR_SFPDB_ERROR;
            }
        }
/*        else if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == ret_status)
        {
            vector<LocationId>locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
            ResourceId localStatus = pWaveSendToClusterContext->getSendStatusForPhase1(locationsVector[0]);
            if (WAVE_MESSAGE_SUCCESS == localStatus)
            {
                localStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1(locationsVector[0]);
            }
            if (FRAMEWORK_SUCCESS == localStatus)
            {
                localStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationsVector[0]);
            }
            ret_status = localStatus;
            locationsVector.clear();
        }
*/
        SystemMonitorLocalSfpMailSetEmailMessage *pSetMsg = (SystemMonitorLocalSfpMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::addEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalSfpMailWorker::SystemMonitorGlobalSfpMailRemoveEmailMessageHandler( SystemMonitorGlobalSfpMailRemoveEmailMessage *pSystemMonitorGlobalSfpMailRemoveEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool removed = false;

        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::SystemMonitorGlobalSfpMailRemoveEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalSfpMailRemoveEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        email = pSystemMonitorGlobalSfpMailRemoveEmailMessage->getEmail();

        // get the managed object list of emails and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(SfpMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. nothing to delete
            trace (TRACE_LEVEL_ERROR, string("SystemMonitorRemoveEmailMessageHandler: email list is empty"));
        }
        else
        {
            // accumulate list into string
            for (unsigned int i = 0, num_emails = 0; i < pResults->size(); i++)
            {
                SfpMailListGlobalManagedObject *pMO = dynamic_cast<SfpMailListGlobalManagedObject *> ((*pResults)[i]);
                string tempEmail = pMO->getEmail();
                if (tempEmail == email)
                {
                    // found email to remove
                    removed = true;
                }
                else
                {
                    if (num_emails)
                    {
                        emaillist += FWEMAIL_TOKEN_SEPERATOR;
                    }
                    emaillist += tempEmail;
                    num_emails++;
                }
                delete (*pResults)[i];
            }
        }

        if (NULL != pResults)
        {
            delete pResults;
        }
        delete syncQuery;

        if (false == removed)
        {
            // email not in list. return status
            status = SYSTEMMONITOR_EMAIL_NOT_IN_LIST;
            trace (TRACE_LEVEL_ERROR, string("GlobalSfpMailWorker::SystemMonitorGlobalSfpMailRemoveEmailMessageHandler email not found. ") + email);
            pSystemMonitorGlobalSfpMailRemoveEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalSfpMailRemoveEmailMessage);
            return;
        }

        // send setemail msg to cluster
        SystemMonitorLocalSfpMailSetEmailMessage *pMessage = new SystemMonitorLocalSfpMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalSfpMailWorker::removeEmailSTCCallback), pSystemMonitorGlobalSfpMailRemoveEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalSfpMailWorker::removeEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::removeEmailSTCCallback"));

        SystemMonitorGlobalSfpMailRemoveEmailMessage *pOrigMsg = (SystemMonitorGlobalSfpMailRemoveEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId ret_status = pWaveSendToClusterContext->getCompletionStatus();
		AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;
		bool allFailed = false;
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                ret_status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "Remove email operation failed on one or more of the connected nodes.");
            else {
                trace (TRACE_LEVEL_ERROR, "Remove email operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalSfpMailWorker::removeEmailSTCCallback", errorMessage, true);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    ret_status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        {
            trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::removeEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // Find and Remove the email from the managed object
            WaveManagedObjectSynchronousQueryContext *syncQueryAttr = new WaveManagedObjectSynchronousQueryContext(SfpMailListGlobalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQueryAttr);
            if (NULL != pResults)
            {
                if (0 == pResults->size ())
                {
                    // no records. internal error
                    ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
                }
                else
                {
                    // look for server 
                    ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
                    for (UI32 i = 0; i < pResults->size (); i++)
                    {
                        SfpMailListGlobalManagedObject *pMO = dynamic_cast<SfpMailListGlobalManagedObject *> ((*pResults)[i]);
                        // look for email and remove it
                        if (email == pMO->getEmail())
                        {
                            startTransaction();
                            delete pMO;
                            ret_status = commitTransaction();
                            break;
                        }
                    }
                }
                pResults->clear ();
                delete pResults;
            }
            else
            {
                // query error
                ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
            }
            delete syncQueryAttr;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::removeEmailSTCCallback: Removed email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::removeEmailSTCCallback: MO failure removing email") + email);
                ret_status = SYSTEMMONITOR_SFPDB_ERROR;
            }
        }
/*        else if (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == ret_status)
        {
            vector<LocationId>locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
            ResourceId localStatus = pWaveSendToClusterContext->getSendStatusForPhase1(locationsVector[0]);
            if (WAVE_MESSAGE_SUCCESS == localStatus)
            {
                localStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1(locationsVector[0]);
            }
            if (FRAMEWORK_SUCCESS == localStatus)
            {
                localStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationsVector[0]);
            }
            ret_status = localStatus;
            locationsVector.clear();
        }
*/
        SystemMonitorLocalSfpMailSetEmailMessage *pSetMsg = (SystemMonitorLocalSfpMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::removeEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalSfpMailWorker::SystemMonitorGlobalSfpMailSetCfgMessageHandler( SystemMonitorGlobalSfpMailSetCfgMessage *pSystemMonitorGlobalSfpMailSetCfgMessage)
    {
        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::SystemMonitorGlobalSfpMailSetCfgMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalSfpMailSetCfgMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        bool flag = pSystemMonitorGlobalSfpMailSetCfgMessage->getEnableFlag();

        // send email enable flag to cluster
        SystemMonitorLocalSfpMailSetCfgMessage *pMessage = new SystemMonitorLocalSfpMailSetCfgMessage (flag);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalSfpMailWorker::setEmailFlagSTCCallback), pSystemMonitorGlobalSfpMailSetCfgMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalSfpMailWorker::setEmailFlagSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::setEmailFlagSTCCallback"));

        SystemMonitorGlobalSfpMailSetCfgMessage *pOrigMsg = (SystemMonitorGlobalSfpMailSetCfgMessage *)pWaveSendToClusterContext->getPCallerContext();
        ResourceId ret_status = pWaveSendToClusterContext->getCompletionStatus();
		AAAChangeRadiusHostContext *pRadiusHostCtx    = NULL;
		bool allFailed = false;
        string errorMessage;

        if ((pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true) ||
                ret_status == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES)
        {    
            if (pWaveSendToClusterContext->getIsPartialSuccessCaseFlag() == true)
                trace (TRACE_LEVEL_ERROR, "Set email flag operation failed on one or more of the connected nodes.");
            else {
                trace (TRACE_LEVEL_ERROR, "Set email flag operation failed on all nodes");
                allFailed = true;
            }

            vector<LocationId> cxLocations;
            FrameworkToolKit::getFullyConnectedLocations(cxLocations);
            if (cxLocations.size() > 0) 
            {
                pRadiusHostCtx->traceSendToClusterErrors
                                (pWaveSendToClusterContext, "GlobalSfpMailWorker::setEmailFlagSTCCallback", errorMessage, true);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "The operation is run only on one node");
                LocationId thisNode = FrameworkToolKit::getThisLocationId();
                if (pWaveSendToClusterContext->getSendStatusForPhase1(thisNode) != WAVE_MESSAGE_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getSendStatusForPhase1(thisNode);
                else if (pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode) != FRAMEWORK_SUCCESS)
                    ret_status = pWaveSendToClusterContext->getFrameworkStatusForPhase1(thisNode);
                else
                    ret_status = pWaveSendToClusterContext->getCompletionStatusForPhase1(thisNode);
            }
        }

        if (!allFailed)
        {
            trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::setEmailFlagSTCCallback : SUCCESS on local plugins. Updating DB"));

            bool flag = pOrigMsg->getEnableFlag();

            WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(SfpMailCfgGlobalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
            if ((NULL == pResults) || (1 < pResults->size ()))
            {   // internal error, should always have just one
                trace (TRACE_LEVEL_ERROR, string("GlobalSfpMailWorker::setEmailFlagSTCCallback : DB SfpMailCfgGlobalManagedObject not found or too many"));
                ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
            }
            else if (0 == pResults->size())
            {   // does not exist.  create managed object
                startTransaction();
                SfpMailCfgGlobalManagedObject *pMO = dynamic_cast<SfpMailCfgGlobalManagedObject *>(createManagedObjectInstance(SfpMailCfgGlobalManagedObject::getClassName()));
                pMO->setEnableFlag(flag);
                ret_status = commitTransaction();
                delete pMO;
            }
            else if (1 == pResults->size())
            {   // found. update flag value
                SfpMailCfgGlobalManagedObject *pMO = dynamic_cast<SfpMailCfgGlobalManagedObject *> ((*pResults)[0]);
                startTransaction();
                updateWaveManagedObject(pMO);
                pMO->setEnableFlag(flag);
                ret_status = commitTransaction();
                delete pMO;
            }

            if (pResults)
            {
                pResults->clear ();
                delete pResults;
            }
            delete syncQuery;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::setEmailFlagSTCCallback: flag set to DB"));
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalSfpMailWorker::setEmailFlagSTCCallback: failed setting flag to DB"));
                ret_status = SYSTEMMONITOR_MAILFLAG_ERROR;
            }
        }
/*        else if ((WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == ret_status) || (true == pWaveSendToClusterContext->getIsPartialSuccessCaseFlag()))
        {
            vector<LocationId>locationsVector = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
			vector<LocationId> cxLocations;
			FrameworkToolKit::getConnectedLocations(cxLocations);
			if (0 == cxLocations.size())
			{
      	      ResourceId localStatus = pWaveSendToClusterContext->getSendStatusForPhase1(locationsVector[0]);
        	    if (WAVE_MESSAGE_SUCCESS == localStatus)
            	{
	                localStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1(locationsVector[0]);
    	        }
        	    if (FRAMEWORK_SUCCESS == localStatus)
            	{
	                localStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1(locationsVector[0]);
    	        }
        	    ret_status = localStatus;
			}
			if (true == pWaveSendToClusterContext->getIsPartialSuccessCaseFlag())
			{
				for (int i = 0; i < locationsVector.size(); i++) {
 					pMessage = dynamic_cast<SystemMonitorLocalIntMailSetCfgMessage *> (ctx_p->getResultingMessageForPhase1(locationsVector[i]));
					prismAssert (pMessage != NULL, __FILE__, __LINE__);
					ResourceId clusterCompletionStatus = pMessage->getCompletionStatus();
					trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
						+ locationsVector[i] + string("returned error: ")
						+ FrameworkToolKit::localize(clusterCompletionStatus));
					break;
				}
			}	
            locationsVector.clear();
        }
*/
        SystemMonitorLocalSfpMailSetCfgMessage *pSetMsg = (SystemMonitorLocalSfpMailSetCfgMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalSfpMailWorker::setEmailFlagSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

}
