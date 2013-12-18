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
#include "SystemMonitor/Global/GlobalIntMailWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalIntMailSetCfgMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalIntMailAddEmailMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalIntMailRemoveEmailMessage.h"
#include "SystemMonitor/Global/IntMailCfgGlobalManagedObject.h"
#include "SystemMonitor/Global/IntMailListGlobalManagedObject.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/SystemMonitorLocalIntMailSetEmailMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalIntMailSetCfgMessage.h"

#include "DcmResourceIdEnums.h"
#include "thresh/fwdPub.h"

#include "AAA/Global/AAASequencerContexts.h"

namespace DcmNs
{

    GlobalIntMailWorker::GlobalIntMailWorker ( SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager)
        : WaveWorker  (pSystemMonitorGlobalObjectManager)
    {
        IntMailCfgGlobalManagedObject    IntMailCfgGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        IntMailCfgGlobalManagedObject.setupOrm ();
        addManagedClass (IntMailCfgGlobalManagedObject::getClassName (), this);
        IntMailListGlobalManagedObject    IntMailListGlobalManagedObject    (pSystemMonitorGlobalObjectManager);
        IntMailListGlobalManagedObject.setupOrm ();
        addManagedClass (IntMailListGlobalManagedObject::getClassName (), this);

        addOperationMap (SYSTEMMONITORGLOBALINTMAILSETCFG, reinterpret_cast<PrismMessageHandler> (&GlobalIntMailWorker::SystemMonitorGlobalIntMailSetCfgMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALINTMAILADDEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalIntMailWorker::SystemMonitorGlobalIntMailAddEmailMessageHandler));
        addOperationMap (SYSTEMMONITORGLOBALINTMAILREMOVEEMAIL, reinterpret_cast<PrismMessageHandler> (&GlobalIntMailWorker::SystemMonitorGlobalIntMailRemoveEmailMessageHandler));
    }

    GlobalIntMailWorker::~GlobalIntMailWorker ()
    {
    }

    PrismMessage  *GlobalIntMailWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case SYSTEMMONITORGLOBALINTMAILSETCFG :
                pPrismMessage = new SystemMonitorGlobalIntMailSetCfgMessage ();
                break;
            case SYSTEMMONITORGLOBALINTMAILADDEMAIL :
                pPrismMessage = new SystemMonitorGlobalIntMailAddEmailMessage ();
                break;
            case SYSTEMMONITORGLOBALINTMAILREMOVEEMAIL :
                pPrismMessage = new SystemMonitorGlobalIntMailRemoveEmailMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *GlobalIntMailWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((IntMailCfgGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IntMailCfgGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IntMailListGlobalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IntMailListGlobalManagedObject(dynamic_cast<SystemMonitorGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "GlobalIntMailWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }

        return (pWaveManagedObject);
    }

    void  GlobalIntMailWorker::SystemMonitorGlobalIntMailAddEmailMessageHandler( SystemMonitorGlobalIntMailAddEmailMessage *pSystemMonitorGlobalIntMailAddEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool added = true;

        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::SystemMonitorGlobalIntMailAddEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalIntMailAddEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL message in handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        email = pSystemMonitorGlobalIntMailAddEmailMessage->getEmail();
        // get list of emails from managed object and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(IntMailListGlobalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
        if ((NULL == pResults) || (0 == pResults->size ()))
        {
            // empty list. list will just be the newly added email
            trace (TRACE_LEVEL_INFO, string("SystemMonitorGlobalIntMailAddEmailMessageHandler: email list now has one entry"));
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
                IntMailListGlobalManagedObject *pMO = dynamic_cast<IntMailListGlobalManagedObject *> ((*pResults)[i]);
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
            pSystemMonitorGlobalIntMailAddEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalIntMailAddEmailMessage);
            return;
        }

        // send email list to cluster
        SystemMonitorLocalIntMailSetEmailMessage *pMessage = new SystemMonitorLocalIntMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIntMailWorker::addEmailSTCCallback), pSystemMonitorGlobalIntMailAddEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalIntMailWorker::addEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalIntMailWorker::addEmailSTCCallback Enter"));

        SystemMonitorGlobalIntMailAddEmailMessage *pOrigMsg = (SystemMonitorGlobalIntMailAddEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalIntMailWorker::addEmailSTCCallback", errorMessage, true);
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
            trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::addEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // add email to the managed object
            startTransaction();
            IntMailListGlobalManagedObject *pMO = dynamic_cast<IntMailListGlobalManagedObject *>(createManagedObjectInstance(IntMailListGlobalManagedObject::getClassName()));
            pMO->setEmail(email);
            ret_status = commitTransaction();
            delete pMO;

            if (FRAMEWORK_SUCCESS == ret_status)
            {
                trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::addEmailSTCCallback added email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalIntMailWorker::addEmailSTCCallback: failed adding email to DB"));
                ret_status = SYSTEMMONITOR_INTDB_ERROR;
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
        SystemMonitorLocalIntMailSetEmailMessage *pSetMsg = (SystemMonitorLocalIntMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::addEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalIntMailWorker::SystemMonitorGlobalIntMailRemoveEmailMessageHandler( SystemMonitorGlobalIntMailRemoveEmailMessage *pSystemMonitorGlobalIntMailRemoveEmailMessage)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string email, emaillist;
        bool removed = false;

        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::SystemMonitorGlobalIntMailRemoveEmailMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalIntMailRemoveEmailMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }
        email = pSystemMonitorGlobalIntMailRemoveEmailMessage->getEmail();

        // get the managed object list of emails and compile into a string
        emaillist.clear();
        WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(IntMailListGlobalManagedObject::getClassName());
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
                IntMailListGlobalManagedObject *pMO = dynamic_cast<IntMailListGlobalManagedObject *> ((*pResults)[i]);
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
            trace (TRACE_LEVEL_ERROR, string("GlobalIntMailWorker::SystemMonitorGlobalIntMailRemoveEmailMessageHandler email not found. ") + email);
            pSystemMonitorGlobalIntMailRemoveEmailMessage->setCompletionStatus(status);
            reply (pSystemMonitorGlobalIntMailRemoveEmailMessage);
            return;
        }

        // send setemail msg to cluster
        SystemMonitorLocalIntMailSetEmailMessage *pMessage = new SystemMonitorLocalIntMailSetEmailMessage (emaillist);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIntMailWorker::removeEmailSTCCallback), pSystemMonitorGlobalIntMailRemoveEmailMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalIntMailWorker::removeEmailSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalIntMailWorker::removeEmailSTCCallback"));

        SystemMonitorGlobalIntMailRemoveEmailMessage *pOrigMsg = (SystemMonitorGlobalIntMailRemoveEmailMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalIntMailWorker::removeEmailSTCCallback", errorMessage, true);
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
            trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::removeEmailSTCCallback : SUCCESS on local plugins. Updating DB"));

            string email = pOrigMsg->getEmail();

            // Find and Remove the email from the managed object
            WaveManagedObjectSynchronousQueryContext *syncQueryAttr = new WaveManagedObjectSynchronousQueryContext(IntMailListGlobalManagedObject::getClassName());
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
                        IntMailListGlobalManagedObject *pMO = dynamic_cast<IntMailListGlobalManagedObject *> ((*pResults)[i]);
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
                trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::removeEmailSTCCallback: Removed email ") + email);
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::removeEmailSTCCallback: MO failure removing email") + email);
                ret_status = SYSTEMMONITOR_INTDB_ERROR;
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
        SystemMonitorLocalIntMailSetEmailMessage *pSetMsg = (SystemMonitorLocalIntMailSetEmailMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::removeEmailSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }


    void  GlobalIntMailWorker::SystemMonitorGlobalIntMailSetCfgMessageHandler( SystemMonitorGlobalIntMailSetCfgMessage *pSystemMonitorGlobalIntMailSetCfgMessage)
    {
        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::SystemMonitorGlobalIntMailSetCfgMessageHandler Enter."));

        // Validations and Queries should go here
        if (pSystemMonitorGlobalIntMailSetCfgMessage == NULL)
        {
            trace (TRACE_LEVEL_FATAL, string("NULL Message arrived in the handler"));
            prismAssert (false, __FILE__, __LINE__);
        }

        bool flag = pSystemMonitorGlobalIntMailSetCfgMessage->getEnableFlag();

        // send email enable flag to cluster
        SystemMonitorLocalIntMailSetCfgMessage *pMessage = new SystemMonitorLocalIntMailSetCfgMessage (flag);

        WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&GlobalIntMailWorker::setEmailFlagSTCCallback), pSystemMonitorGlobalIntMailSetCfgMessage);
        pWaveSendToClusterContext->setPartialSuccessFlag(true);
        pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessage);

        sendToWaveCluster (pWaveSendToClusterContext);
    }

    void GlobalIntMailWorker::setEmailFlagSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
    {
        trace(TRACE_LEVEL_INFO, string("GlobalIntMailWorker::setEmailFlagSTCCallback"));

        SystemMonitorGlobalIntMailSetCfgMessage *pOrigMsg = (SystemMonitorGlobalIntMailSetCfgMessage *)pWaveSendToClusterContext->getPCallerContext();
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
                                (pWaveSendToClusterContext, "GlobalIntMailWorker::setEmailFlagSTCCallback", errorMessage, true);
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
            trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::setEmailFlagSTCCallback : SUCCESS on local plugins. Updating DB"));

            bool flag = pOrigMsg->getEnableFlag();

            WaveManagedObjectSynchronousQueryContext *syncQuery = new WaveManagedObjectSynchronousQueryContext(IntMailCfgGlobalManagedObject::getClassName());
            vector<WaveManagedObject *> *pResults = querySynchronously (syncQuery);
            if ((NULL == pResults) || (1 < pResults->size ()))
            {   // internal error, should always have just one
                trace (TRACE_LEVEL_ERROR, string("GlobalIntMailWorker::setEmailFlagSTCCallback : DB IntMailCfgGlobalManagedObject not found or too many"));
                ret_status = SYSTEMMONITOR_INTERNAL_ERROR;
            }
            else if (0 == pResults->size())
            {   // does not exist.  create managed object
                startTransaction();
                IntMailCfgGlobalManagedObject *pMO = dynamic_cast<IntMailCfgGlobalManagedObject *>(createManagedObjectInstance(IntMailCfgGlobalManagedObject::getClassName()));
                pMO->setEnableFlag(flag);
                ret_status = commitTransaction();
                delete pMO;
            }
            else if (1 == pResults->size())
            {   // found. update flag value
                IntMailCfgGlobalManagedObject *pMO = dynamic_cast<IntMailCfgGlobalManagedObject *> ((*pResults)[0]);
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
                trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::setEmailFlagSTCCallback: flag set to DB"));
                ret_status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string("GlobalIntMailWorker::setEmailFlagSTCCallback: failed setting flag to DB"));
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
        SystemMonitorLocalIntMailSetCfgMessage *pSetMsg = (SystemMonitorLocalIntMailSetCfgMessage *)pWaveSendToClusterContext->getPPrismMessageForPhase1();

        delete(pSetMsg);
        delete pWaveSendToClusterContext;

        trace (TRACE_LEVEL_INFO, string("GlobalIntMailWorker::setEmailFlagSTCCallback Exiting"));
		pOrigMsg->setMessageString (errorMessage);
        pOrigMsg->setCompletionStatus(ret_status);
        reply (pOrigMsg);
    }

}
