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

#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalFruMailWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalFruMailSetCfgMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalFruMailAddEmailMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalFruMailRemoveEmailMessage.h"
#include "SystemMonitor/Global/FruMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/FruMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalFruMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalFruMailSetCfgMessage.h"

#include "DcmResourceIdEnums.h"
#include "thresh/fwdPub.h"

#include "AAA/Global/AAASequencerContexts.h"

namespace DcmNs
{

    GlobalFruMailWorker::GlobalFruMailWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        FruMailCfgGlobalManagedObject    FruMailCfgGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        FruMailCfgGlobalManagedObject.setupOrm ();
        addManagedClass (FruMailCfgGlobalManagedObject::getClassName (), this);
        FruMailListGlobalManagedObject    FruMailListGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        FruMailListGlobalManagedObject.setupOrm ();
        addManagedClass (FruMailListGlobalManagedObject::getClassName (), this);

        addOperationMap (SYSTEMMONITORGLOBALFRUMAILSETCFG, reinterpret_cast<PrismMessageHandler> (&GlobalFruMailWorker::SystemMonitorGlobalFruMailSetCfgMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALFRUMAILADDEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalFruMailWorker::SystemMonitorGlobalFruMailAddEmailMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALFRUMAILREMOVEEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalFruMailWorker::SystemMonitorGlobalFruMailRemoveEmailMessageHandler));
    }

    GlobalFruMailWorker::~GlobalFruMailWorker ()
    {
    }

    PrismMessage  *GlobalFruMailWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALFRUMAILSETCFG :
                pPrismMessage = new SystemMonitorGlobalFruMailSetCfgMessage ();
                break;
            case SYSTEMMONITORGLOBALFRUMAILADDEMAIL :
                pPrismMessage = new SystemMonitorGlobalFruMailAddEmailMessage ();
                break;
            case SYSTEMMONITORGLOBALFRUMAILREMOVEEMAIL :
                pPrismMessage = new SystemMonitorGlobalFruMailRemoveEmailMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalFruMailWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((FruMailCfgGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FruMailCfgGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FruMailListGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FruMailListGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalFruMailWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }

    void  GlobalFruMailWorker::SystemMonitorGlobalFruMailAddEmailMessageHandler( SystemMonitorGlobalFruMailAddEmailMessage *pSystemMonitorGlobalFruMailAddEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::SystemMonitorGlobalFruMailAddEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalFruMailAddEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL message in handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        email = pSystemMonitorGlobalFruMailAddEmailMessage->getEmail();
        // get list of emails from managed object and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(FruMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added email
            trace (TRACE_LEVEL_INFO, string("SystemMonitorGlobalFruMailAddEmailMessageHandler: email list now has one entry"));
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
                FruMailListGlobalManagedObject *pMO = dynamic_cast<FruMailListGlobalManagedObject *> ((*pResults)[i]);
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
            pSystemMonitorGlobalFruMailAddEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalFruMailAddEmailMessage);
            return;
        }

        // send email list to cluster
        SystemMonitorLocalFruMailSetEmailMessage *pMessage = new SystemMonitorLocalFruMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalFruMailWorker::addEmailSTCCallback), pSystemMonitorGlobalFruMailAddEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalFruMailWorker::addEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalFruMailWorker::addEmailSTCCallback Enter"));

        SystemMonitorGlobalFruMailAddEmailMessage *pOrigMsg = (SystemMonitorGlobalFruMailAddEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalFruMailWorker::addEmailSTCCallback", errorMessage, true);
				trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::addEmailSTCCallback:traceSendToClusterErrors") + errorMessage);
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
            trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::addEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // add email to the managed object
            startTransaction();
            FruMailListGlobalManagedObject *pMO = dynamic_cast<FruMailListGlobalManagedObject *>(createManagedObjectInstance(FruMailListGlobalManagedObject::getClassName()));
            pMO->setEmail(email);
            ret_status = commitTransaction();
            delete pMO;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::addEmailSTCCallback added email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalFruMailWorker::addEmailSTCCallback: failed adding email to DB"));
                ret_status = SYSTEMMONITOR_FRUDB_ERROR;
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
        SystemMonitorLocalFruMailSetEmailMessage *pSetMsg = (SystemMonitorLocalFruMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::addEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalFruMailWorker::SystemMonitorGlobalFruMailRemoveEmailMessageHandler( SystemMonitorGlobalFruMailRemoveEmailMessage *pSystemMonitorGlobalFruMailRemoveEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool removed = false;

        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::SystemMonitorGlobalFruMailRemoveEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalFruMailRemoveEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        email = pSystemMonitorGlobalFruMailRemoveEmailMessage->getEmail();

        // get the managed object list of emails and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(FruMailListGlobalManagedObject::getClassName());
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
                FruMailListGlobalManagedObject *pMO = dynamic_cast<FruMailListGlobalManagedObject *> ((*pResults)[i]);
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
            trace (TRACE_LEVEL_ERROR, string("GlobalFruMailWorker::SystemMonitorGlobalFruMailRemoveEmailMessageHandler email not found. ") + email);
            pSystemMonitorGlobalFruMailRemoveEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalFruMailRemoveEmailMessage);
            return;
        }

        // send setemail msg to cluster
        SystemMonitorLocalFruMailSetEmailMessage *pMessage = new SystemMonitorLocalFruMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalFruMailWorker::removeEmailSTCCallback), pSystemMonitorGlobalFruMailRemoveEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalFruMailWorker::removeEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback"));

        SystemMonitorGlobalFruMailRemoveEmailMessage *pOrigMsg = (SystemMonitorGlobalFruMailRemoveEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalFruMailWorker::removeEmailSTCCallback", errorMessage, true);
				trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback:traceSendToClusterErrors") + errorMessage);
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
            trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // Find and Remove the email from the managed object
            WaveManagedObjectSynchronousQueryContext *syncQueryAttr = new WaveManagedObjectSynchronousQueryContext(FruMailListGlobalManagedObject::getClassName());
            syncQueryAttr->addAndAttribute (new AttributeString (email, "email"));
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
                    FruMailListGlobalManagedObject *pMO = dynamic_cast<FruMailListGlobalManagedObject *> ((*pResults)[0]);
                    if (pMO != NULL)
                    {
                        startTransaction();
                        delete pMO;
                        ret_status = commitTransaction();
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
                trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback: Removed email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback: MO failure removing email") + email);
                ret_status = SYSTEMMONITOR_FRUDB_ERROR;
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
        SystemMonitorLocalFruMailSetEmailMessage *pSetMsg = (SystemMonitorLocalFruMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::removeEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalFruMailWorker::SystemMonitorGlobalFruMailSetCfgMessageHandler( SystemMonitorGlobalFruMailSetCfgMessage *pSystemMonitorGlobalFruMailSetCfgMessage)
    {
        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::SystemMonitorGlobalFruMailSetCfgMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalFruMailSetCfgMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        bool flag = pSystemMonitorGlobalFruMailSetCfgMessage->getEnableFlag();

        // send email enable flag to cluster
        SystemMonitorLocalFruMailSetCfgMessage *pMessage = new SystemMonitorLocalFruMailSetCfgMessage (flag);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalFruMailWorker::setEmailFlagSTCCallback), pSystemMonitorGlobalFruMailSetCfgMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalFruMailWorker::setEmailFlagSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalFruMailWorker::setEmailFlagSTCCallback"));

        SystemMonitorGlobalFruMailSetCfgMessage *pOrigMsg = (SystemMonitorGlobalFruMailSetCfgMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalFruMailWorker::setEmailFlagSTCCallback", errorMessage, true);
				trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::setEmailFlagSTCCallback:traceSendToClusterErrors") + errorMessage);
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
            trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::setEmailFlagSTCCallback : SUCCESS on local plugins. Updating DB"));

            bool flag = pOrigMsg->getEnableFlag();

            WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(FruMailCfgGlobalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
            if ((NULL == pResults) || (1 < pResults->size ()))
            {   // internal error, should always have just one
                trace (TRACE_LEVEL_ERROR, string("GlobalFruMailWorker::setEmailFlagSTCCallback : DB FruMailCfgGlobalManagedObject not found or too many"));
                ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
            }
            else if (0 == pResults->size())
            {   // does not exist.  create managed object
                startTransaction();
                FruMailCfgGlobalManagedObject *pMO = dynamic_cast<FruMailCfgGlobalManagedObject *>(createManagedObjectInstance(FruMailCfgGlobalManagedObject::getClassName()));
                pMO->setEnableFlag(flag);
                ret_status = commitTransaction();
                delete pMO;
            }
            else if (1 == pResults->size())
            {   // found. update flag value
                FruMailCfgGlobalManagedObject *pMO = dynamic_cast<FruMailCfgGlobalManagedObject *> ((*pResults)[0]);
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
                trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::setEmailFlagSTCCallback: flag set to DB"));
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalFruMailWorker::setEmailFlagSTCCallback: failed setting flag to DB"));
                ret_status = SYSTEMMONITOR_MAILFLAG_ERROR;
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
        SystemMonitorLocalFruMailSetCfgMessage *pSetMsg = (SystemMonitorLocalFruMailSetCfgMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalFruMailWorker::setEmailFlagSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

}
