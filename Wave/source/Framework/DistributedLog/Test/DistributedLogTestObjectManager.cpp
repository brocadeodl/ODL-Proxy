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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/DistributedLog/Test/DistributedLogTestObjectManager.h"
#include "Framework/DistributedLog/Test/DistributedLogTestMessages.h"
#include "Framework/DistributedLog/Test/DistributedLogTestContext.h"
#include "Framework/DistributedLog/DistributedLogAddLogEntryMessage.h"
#include "Framework/DistributedLog/DistributedLogUpdateMaxLogEntriesMessage.h"
#include "Framework/DistributedLog/DistributedLogConfigurationManagedObject.h"
#include "Framework/DistributedLog/DistributedLogEntryManagedObject.h"
#include "Framework/DistributedLog/DistributedLogTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{
    static const UI64 BIST_MAX_LOG_ENTRIES = 1000;
    static const UI32 BIST_NUMBER_OF_LOGS_TO_ADD = 100;


DistributedLogTestObjectManager::DistributedLogTestObjectManager ()
    : PrismTestObjectManager (getServiceName())
{
    addOperationMap (DISTRIBUTED_LOG_INVOKE_ADD_LOG_API, reinterpret_cast<PrismMessageHandler> (&DistributedLogTestObjectManager::invokeAddLogApiMessageHandler));
}

DistributedLogTestObjectManager::~DistributedLogTestObjectManager ()
{
}

DistributedLogTestObjectManager *DistributedLogTestObjectManager::getInstance ()
{
    static DistributedLogTestObjectManager *pDistributedLogTestObjectManager = new DistributedLogTestObjectManager ();

    WaveNs::prismAssert (NULL != pDistributedLogTestObjectManager, __FILE__, __LINE__);

    return (pDistributedLogTestObjectManager);
}

PrismServiceId DistributedLogTestObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

string DistributedLogTestObjectManager::getServiceName ()
{
    return ("Distributed Log Test");
}

PrismMessage *DistributedLogTestObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DISTRIBUTED_LOG_INVOKE_ADD_LOG_API:
            pPrismMessage = new DistributedLogInvokeAddLogApiMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
} 

void DistributedLogTestObjectManager::invokeAddLogApiMessageHandler (DistributedLogInvokeAddLogApiMessage *pMessage)
{
    ResourceId          logType             = pMessage->getLogType ();
    ResourceId          logDescriptionType  = pMessage->getLogDescriptionType ();
    ObjectId            managedObjectId     = pMessage->getManagedObjectId ();
    UI32                numberOfLogsToAdd   = pMessage->getNumberOfLogsToAdd ();
    UI32                session             = 0;
    ResourceId          addLogStatus        = DISTRIBUTED_LOG_ERROR;         
    ResourceId          completionStatus    = WAVE_MESSAGE_SUCCESS;

    vector<Attribute *> logDescriptionArguments;
    logDescriptionArguments.clear ();

    if (DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGIN == logDescriptionType || DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGOUT == logDescriptionType)
    {
        logDescriptionArguments.push_back (new AttributeString ("bist_user", "username"));      
        logDescriptionArguments.push_back (new AttributeString ("127.0.0.1", "ip"));      
        logDescriptionArguments.push_back (new AttributeString ("con0", "terminal"));      
        logDescriptionArguments.push_back (new AttributeUI32 (session, "session"));      
    }
    else if (DISTRIBUTED_LOG_SUB_TYPE_COMMAND == logDescriptionType)
    {
        logDescriptionArguments.push_back (new AttributeString ("bist_user", "username"));
        logDescriptionArguments.push_back (new AttributeString ("BIST test command", "command"));
        logDescriptionArguments.push_back (new AttributeUI32 (session, "session"));
        logDescriptionArguments.push_back (new AttributeString ("SUCCESS", "result"));
    }
    else if (DISTRIBUTED_LOG_SUB_TYPE_MANAGED_OBJECT_ENTRY == logDescriptionType)
    {
        logDescriptionArguments.push_back (new AttributeObjectId (managedObjectId, "managedObjectId"));
        logDescriptionArguments.push_back (new AttributeString ("BIST test managed object log description", "description"));
    }
    else
    {
        logDescriptionArguments.push_back (new AttributeString ("BIST test generic log description.", "description"));
    }

    for (UI32 i = 0; i < numberOfLogsToAdd; i++)
    {
        addLogStatus = addLog (logType, logDescriptionType, logDescriptionArguments);

        if (DISTRIBUTED_LOG_SUCCESS != addLogStatus)
        {
            completionStatus = WAVE_MESSAGE_ERROR;
            break;
        }
        else
        {
            completionStatus = WAVE_MESSAGE_SUCCESS;
        }
    }

    for (UI32 j = 0; j < static_cast<UI32> (logDescriptionArguments.size ()); j++)
    {
        delete logDescriptionArguments[j];
    }

    logDescriptionArguments.clear ();

    pMessage->setCompletionStatus (completionStatus);
    reply (pMessage);
}

void DistributedLogTestObjectManager::testRequestHandler (RegressionTestMessage *pMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addMaximumLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::verifyMaximumLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::rotateLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::verifyRotatedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::decreaseMaxAllowedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::verifyDecreasedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::increaseMaxAllowedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::verifyUnchangedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addMaximumLogEntriesUsingSendOneWayApiTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addAuditLog1UsingSendToWaveClusterTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addAuditLog2UsingSendToWaveClusterTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addAuditLog3UsingSendToWaveClusterTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::addManagedObjectLog1UsingSendToWaveClusterTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::restoreMaxAllowedLogEntriesTestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedLogTestObjectManager::prismLinearSequencerFailedStep),
    };

    DistributedLogTestContext *pDistributedLogTestContext = new DistributedLogTestContext (pMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogTestContext->start ();
}

void DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    DistributedLogConfigurationManagedObject *pDistributedLogConfigurationManagedObject = NULL;
    UI64                                      maxLogEntriesFromConfiguration            = 0;
    UI64                                      numberOfLogEntries                        = 0;
    UI64                                      firstLogId                                = 0;
    UI64                                      lastLogId                                 = 0;
    WaveMessageStatus                         status                                    = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, "Starting Dump Stats Test.");


    // Query configuration data
    WaveManagedObjectSynchronousQueryContext configurationSynchronousQueryContext (DistributedLogConfigurationManagedObject::getClassName());

    vector<WaveManagedObject *> *pConfigurationResults = querySynchronously (&configurationSynchronousQueryContext);

    UI64 numberOfConfigurationResults = 0;

    if (NULL != pConfigurationResults)
    {
        numberOfConfigurationResults = pConfigurationResults->size ();

        if (1 == numberOfConfigurationResults)
        {
            pDistributedLogConfigurationManagedObject = dynamic_cast<DistributedLogConfigurationManagedObject *> ((*pConfigurationResults)[0]);

            maxLogEntriesFromConfiguration   = pDistributedLogConfigurationManagedObject->getMaxLogEntries ();

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Could not find the single instance of DistributedLogConfigurationManagedObject.");
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pConfigurationResults);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Query failed for DistributedLogConfigurationManagedObject.");
    }

    // Query distributed log for the count and indicies

    ResourceId queryStatus = queryForLogEntryInformation (numberOfLogEntries, firstLogId, lastLogId);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        pDistributedLogTestContext->executeNextStep (queryStatus);    

        return;
    }

    // Save original max log entries so that is can be restored after BIST is completed.
    pDistributedLogTestContext->setOriginalMaxAllowedLogEntries (maxLogEntriesFromConfiguration);

    trace (TRACE_LEVEL_INFO, string ("________________________________________________________________"));
    trace (TRACE_LEVEL_INFO, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Display current config."));
    trace (TRACE_LEVEL_INFO, string ("________________________________________________________________"));
    trace (TRACE_LEVEL_INFO, string ("Config MaxLogEntries : ") + maxLogEntriesFromConfiguration);
    trace (TRACE_LEVEL_INFO, string ("NumberOfLogEntries   : ") + numberOfLogEntries); 
    trace (TRACE_LEVEL_INFO, string ("firstLogId           : ") + firstLogId); 
    trace (TRACE_LEVEL_INFO, string ("lastLogId            : ") + lastLogId); 
    trace (TRACE_LEVEL_INFO, string ("________________________________________________________________"));

    // Remove all but 1 log entries
    trace (TRACE_LEVEL_INFO, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Deleting existing log entries (all but 1)..."));

    DistributedLogUpdateMaxLogEntriesMessage message (1);
    
    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Synchronously updating max allowed log entries failed to be processed. completionStatus = ") + completionStatus + ".");

            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Synchronously updating max allowed log entries failed to be sent. status = ") + status + ".");

        status = WAVE_MESSAGE_ERROR;
    }

    // Set BIST to run with lower max log entries to shorten length of BIST tests.
    trace (TRACE_LEVEL_INFO, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Setting max log entries to ") + BIST_MAX_LOG_ENTRIES + " for BIST run.");

    DistributedLogUpdateMaxLogEntriesMessage defaultBistMessage (BIST_MAX_LOG_ENTRIES);

    status = sendSynchronously (&defaultBistMessage);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = defaultBistMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Synchronously updating max allowed log entries failed to be processed. completionStatus = ") + completionStatus + ".");

            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::dumpAndSaveOriginalStatsTestStep : Synchronously updating max allowed log entries failed to be sent. status = ") + status + ".");

        status = WAVE_MESSAGE_ERROR;
    }
 
    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    DistributedLogConfigurationManagedObject *pDistributedLogConfigurationManagedObject = NULL;
    WaveMessageStatus                         status                                    = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, "Starting Query For Current Distributed Log Configuration Information Test.");

    // Query Log Entries for current first and last logIds.

    UI64 firstLogId = 0;
    UI64 lastLogId  = 0;

    ResourceId queryForFistLogIdStatus = queryForFirstLogId (firstLogId);

    if (WAVE_MESSAGE_SUCCESS != queryForFistLogIdStatus)
    {
        pDistributedLogTestContext->executeNextStep (status);

        return;
    }

    ResourceId queryForLastLogIdStatus = queryForLastLogId (lastLogId);

    if (WAVE_MESSAGE_SUCCESS != queryForLastLogIdStatus)
    {
        pDistributedLogTestContext->executeNextStep (status);

        return;
    }

    pDistributedLogTestContext->setFirstLogId   (firstLogId);
    pDistributedLogTestContext->setNextLogId    (++lastLogId);

    // Query database and store original maximum number of log entries to be restored at the end of test suite

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DistributedLogConfigurationManagedObject::getClassName());

    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    UI64 numberOfResults = 0;

    if (NULL != pResults)
    {
        numberOfResults = pResults->size ();

        if (1 == numberOfResults)
        {
            pDistributedLogConfigurationManagedObject = dynamic_cast<DistributedLogConfigurationManagedObject *> ((*pResults)[0]);

            UI64        maxLogEntries   = pDistributedLogConfigurationManagedObject->getMaxLogEntries ();
            ObjectId    objectId        = pDistributedLogConfigurationManagedObject->getObjectId      ();
            
            pDistributedLogTestContext->setCurrentMaxAllowedLogEntries  (maxLogEntries);
            pDistributedLogTestContext->setManagedObjectId              (objectId);
            pDistributedLogTestContext->setPrismServiceId               (getPrismServiceId ());

            trace (TRACE_LEVEL_INFO, string ("DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep : MaxLogEntries : ") + maxLogEntries + ", FirstLogId : " + firstLogId + ", NextLogId : " + lastLogId + ".");

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep : Could not find the single instance of DistributedLogConfigurationManagedObject.");
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForCurrentConfigInfoTestStep : Query failed for DistributedLogConfigurationManagedObject.");
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::addMaximumLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                 maxLogEntries              = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                 firstLogId                 = pDistributedLogTestContext->getFirstLogId ();
    UI64                 lastLogId                  = pDistributedLogTestContext->getNextLogId ();
    UI64                 numberOfLogEntries         = lastLogId - firstLogId; 
    UI64                 numberOfLogEntriesToAdd    = maxLogEntries - numberOfLogEntries;
    PrismServiceId       prismServiceId             = pDistributedLogTestContext->getPrismServiceId ();
    ObjectId             configInfoObjectId         = pDistributedLogTestContext->getManagedObjectId ();
    WaveMessageStatus    status                     = WAVE_MESSAGE_ERROR;


    trace (TRACE_LEVEL_INFO, string ("Starting Add Maximum Log Entries (") + numberOfLogEntriesToAdd + ") Test.");

    for (UI64 i = 0; i < numberOfLogEntriesToAdd; i++)
    {
        DistributedLogAddLogEntryMessage message;
        message.setDescription      (string ("This is the add maximum logs description : ") + i);
        message.setUsername         ("root");
        message.setPrismServiceId   (prismServiceId);
        message.setManagedObjectId  (configInfoObjectId);

        status = sendSynchronously (&message);       
            
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::addMaximumLogEntriesTestStep : Synchronously adding log entry failed to be processed. completionStatus = ") + completionStatus + ".");
                
                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::addMaximumLogEntriesTestStep : Synchronously adding log entry failed to be sent. status = ") + status + ".");
            
            status = WAVE_MESSAGE_ERROR;
            break;
        }
    }
    
    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::verifyMaximumLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                numberOfLogEntries  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                numberOfResults     = 0;
    ResourceId          status              = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Verify Maximum Log Entries (") + numberOfLogEntries + ") Test.");

    // Query database to verify number of log entries

    status = queryForNumberOfLogEntries (numberOfResults);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyMaximumLogEntriesTestStep : Expected and actual number of log entries mismatch. Expected: ") + numberOfLogEntries + ", Actual: " + numberOfResults + ".");
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::rotateLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64               numberOfLogEntries   = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    PrismServiceId     prismServiceId       = pDistributedLogTestContext->getPrismServiceId ();
    ObjectId           objectId             = pDistributedLogTestContext->getManagedObjectId ();
    WaveMessageStatus  status               = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Rotate Log Entries (") + numberOfLogEntries + ") Test.");

    for (UI64 i = 0; i < numberOfLogEntries; i++)
    {
        DistributedLogAddLogEntryMessage    message;
        message.setUsername                 ("admin");
        message.setDescription              (string ("This is the rotate logs description : ") + i);
        message.setPrismServiceId           (prismServiceId);
        message.setManagedObjectId          (objectId);

        status = sendSynchronously (&message);
    
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::addMaximumLogEntriesTestStep : Synchronously adding log entry failed to be processed. completionStatus = ") + completionStatus + ".");

                status = WAVE_MESSAGE_ERROR;
                break;
            }
        }
        else 
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::addMaximumLogEntriesTestStep : Synchronously adding log entry failed to be sent. status = ") + status + ".");
            
            status = WAVE_MESSAGE_ERROR;
            break;
        }
    }
 
    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::verifyRotatedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                                numberOfLogEntries                  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                                firstLogId                          = pDistributedLogTestContext->getFirstLogId ();
    UI64                                nextLogId                           = pDistributedLogTestContext->getNextLogId ();
    UI64                                numberOfResults                     = 0;
    UI64                                newFirstLogId                       = 0;
    UI64                                newNextLogId                        = 0;
    WaveMessageStatus                   status                              = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Verify Rotated Log Entries (") + numberOfLogEntries + ") Test.");

    // Query distributed log for the count and indicies

    UI64 lastLogId = 0;

    ResourceId queryStatus = queryForLogEntryInformation (numberOfResults, newFirstLogId, lastLogId);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        pDistributedLogTestContext->executeNextStep (queryStatus);    

        return;
    }
    else
    {
        newNextLogId = lastLogId + 1;
    }

    // Verify count and indicies

    UI64 expectedFirstLogId = firstLogId + numberOfLogEntries;
    UI64 expectedNextLogId  = nextLogId + numberOfLogEntries;

    if (numberOfResults == numberOfLogEntries)
    {
        if (newFirstLogId == expectedFirstLogId)
        {
            if (newNextLogId == expectedNextLogId)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyRotatedLogEntriesTestStep : Expected and actual nextLogId index mismatch. Expected: ") + expectedNextLogId + ", Actual: " + newNextLogId + ".");
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyRotatedLogEntriesTestStep : Expected and actual firstLogId index mismatch. Expected: ") + expectedFirstLogId  + ", Actual: " + newFirstLogId + ".");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyRotatedLogEntriesTestStep : Expected and actual number of log entries mismatch. Expected: ") + numberOfLogEntries + ", Actual: " + numberOfResults + ".");
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::decreaseMaxAllowedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64        numberOfLogEntries  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64        halvedLogEntries    = numberOfLogEntries / 2;
    ResourceId  status              = WAVE_MESSAGE_ERROR;


    trace (TRACE_LEVEL_INFO, string ("Starting Decrease Maximum Allowed Log Entries (") + halvedLogEntries + ") Test.");

    DistributedLogUpdateMaxLogEntriesMessage message (halvedLogEntries);

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::decreaseMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be processed. completionStatus = ") + completionStatus + ".");

            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::decreaseMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be sent. status = ") + status + ".");

        status = WAVE_MESSAGE_ERROR;
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::verifyDecreasedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                                numberOfLogEntries                  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                                halvedLogEntries                    = numberOfLogEntries / 2;
    UI64                                numberOfRemovedEntries              = numberOfLogEntries - halvedLogEntries;
    UI64                                firstLogId                          = pDistributedLogTestContext->getFirstLogId ();
    UI64                                nextLogId                           = pDistributedLogTestContext->getNextLogId ();
    UI64                                numberOfResults                     = 0;
    UI64                                newFirstLogId                       = 0;
    UI64                                newNextLogId                        = 0;
    WaveMessageStatus                   status                              = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Verify Decreased Log Entries (") + halvedLogEntries + ") Test.");

    // Query distributed log for the count and indicies

    UI64 lastLogId = 0;

    ResourceId queryStatus = queryForLogEntryInformation (numberOfResults, newFirstLogId, lastLogId);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        pDistributedLogTestContext->executeNextStep (queryStatus);

        return;
    }
    else
    {
        newNextLogId = lastLogId + 1;
    }

    // Verify count and indicies

    UI64 expectedFirstLogId = firstLogId + numberOfRemovedEntries;
    UI64 expectedNextLogId  = nextLogId;

    if (halvedLogEntries == numberOfResults)
    {
        if (newFirstLogId == expectedFirstLogId)
        {
            if (newNextLogId == expectedNextLogId)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyDecreasedLogEntriesTestStep : Expected and actual nextLogId index mismatch. Expected: ") + expectedNextLogId + ", Actual: " + newNextLogId + ".");
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyDecreasedLogEntriesTestStep : Expected and actual firstLogId index mismatch. Expected: ") + expectedFirstLogId  + ", Actual: " + newFirstLogId + ".");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyDecreaseLogEntriesTestStep : Expected and actual number of log entries mismatch. Expected: ") + numberOfLogEntries + ", Actual: " + numberOfResults + ".");
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::increaseMaxAllowedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64        numberOfLogEntries  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64        doubledLogEntries   = numberOfLogEntries * 2;
    ResourceId  status              = WAVE_MESSAGE_ERROR;


    trace (TRACE_LEVEL_INFO, string ("Starting Increase Maximum Allowed Log Entries (") + doubledLogEntries + ") Test.");

    DistributedLogUpdateMaxLogEntriesMessage message (doubledLogEntries);

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::increaseMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be processed. completionStatus = ") + completionStatus + ".");

            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::increaseMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be sent. status = ") + status + ".");

        status = WAVE_MESSAGE_ERROR;
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::verifyUnchangedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                                numberOfLogEntries                  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                                halvedLogEntries                    = numberOfLogEntries / 2;
    UI64                                numberOfRemovedEntries              = numberOfLogEntries - halvedLogEntries;
    UI64                                firstLogId                          = pDistributedLogTestContext->getFirstLogId ();
    UI64                                nextLogId                           = pDistributedLogTestContext->getNextLogId ();
    UI64                                numberOfResults                     = 0;
    UI64                                newFirstLogId                       = 0;
    UI64                                newNextLogId                        = 0;
    WaveMessageStatus                   status                              = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Verify Unchanged Log Entries (") + halvedLogEntries + ") Test.");

    // Query distributed log for the count and indicies

    UI64 lastLogId = 0;

    ResourceId queryStatus = queryForLogEntryInformation (numberOfResults, newFirstLogId, lastLogId);

    if (WAVE_MESSAGE_SUCCESS != queryStatus)
    {
        pDistributedLogTestContext->executeNextStep (queryStatus);

        return;
    }
    else
    {
        newNextLogId = lastLogId + 1;
    }

    // Verify count and indicies

    UI64 expectedFirstLogId = firstLogId + numberOfRemovedEntries;
    UI64 expectedNextLogId  = nextLogId;

    if (halvedLogEntries == numberOfResults)
    {
        if (newFirstLogId == expectedFirstLogId)
        {
            if (newNextLogId == expectedNextLogId)
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyUnchangedLogEntriesTestStep : Expected and actual nextLogId index mismatch. Expected: ") + expectedNextLogId + ", Actual: " + newNextLogId + ".");
            }
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyUnchangedLogEntriesTestStep : Expected and actual firstLogId index mismatch. Expected: ") + expectedFirstLogId  + ", Actual: " + newFirstLogId + ".");
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::verifyUnchangedLogEntriesTestStep : Expected and actual number of log entries mismatch. Expected: ") + numberOfLogEntries + ", Actual: " + numberOfResults + ".");
    }

    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::addMaximumLogEntriesUsingSendOneWayApiTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64                 numberOfLogEntries  = pDistributedLogTestContext->getCurrentMaxAllowedLogEntries ();
    UI64                 doubledLogEntries   = numberOfLogEntries * 2;
    LogType              logType             = DISTRIBUTED_LOG_TYPE_GENERIC_LOG;
    LogDescriptionType   logDescriptionType  = DISTRIBUTED_LOG_SUB_TYPE_GENERIC_ENTRY;
    ResourceId           addLogStatus        = DISTRIBUTED_LOG_ERROR;
    ResourceId           status              = WAVE_MESSAGE_ERROR;


    trace (TRACE_LEVEL_INFO, string ("Starting Add Maximum Log Entries Using Send One Way API (") + doubledLogEntries + ") Test.");

    pDistributedLogTestContext->setNumberOfFailures (0);

    ++(*pDistributedLogTestContext);

    for (UI64 i = 0; i < doubledLogEntries; i++)
    {
        vector<Attribute *> arguments;
        arguments.clear ();
        arguments.push_back (new AttributeString (string ("This is the add maximum log entries using send one way API : ") + i, "logDescription"));

        addLogStatus = addLog (logType, logDescriptionType, arguments);
        
        if (DISTRIBUTED_LOG_SUCCESS != addLogStatus)
        {
            pDistributedLogTestContext->incrementNumberOfFailures ();
        }

        delete arguments[0];
        arguments.clear ();
    }

    --(*pDistributedLogTestContext);

    if (0 != pDistributedLogTestContext->getNumberOfFailures ())
    {       
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::addMaximumLogEntriesUsingSendOneWayApiTestStep : Failed to send one way using api : ") + pDistributedLogTestContext->getNumberOfFailures ());

        status = WAVE_MESSAGE_ERROR;
    }
    else
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    
    pDistributedLogTestContext->executeNextStep (status);
}

void DistributedLogTestObjectManager::addAuditLog1UsingSendToWaveClusterTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    trace (TRACE_LEVEL_INFO, string ("Starting Add Audit Session Login Using Send To Wave Cluster (") + BIST_NUMBER_OF_LOGS_TO_ADD + ") Test.");

    LogType             logType             = DISTRIBUTED_LOG_TYPE_AUDIT_LOG;
    LogDescriptionType  logDescriptionType  = DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGIN;
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&DistributedLogTestObjectManager::addLogUsingSendToWaveClusterTestCallback), pDistributedLogTestContext);

    DistributedLogInvokeAddLogApiMessage *pMessage1 = new DistributedLogInvokeAddLogApiMessage ();
    pMessage1->setLogType (logType);
    pMessage1->setLogDescriptionType (logDescriptionType);
    pMessage1->setNumberOfLogsToAdd (BIST_NUMBER_OF_LOGS_TO_ADD);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage1);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void DistributedLogTestObjectManager::addAuditLog2UsingSendToWaveClusterTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    trace (TRACE_LEVEL_INFO, string ("Starting Add Audit Command Using Send To Wave Cluster (") + BIST_NUMBER_OF_LOGS_TO_ADD + ") Test.");

    LogType             logType             = DISTRIBUTED_LOG_TYPE_AUDIT_LOG;
    LogDescriptionType  logDescriptionType  = DISTRIBUTED_LOG_SUB_TYPE_COMMAND;
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&DistributedLogTestObjectManager::addLogUsingSendToWaveClusterTestCallback), pDistributedLogTestContext);

    DistributedLogInvokeAddLogApiMessage *pMessage1 = new DistributedLogInvokeAddLogApiMessage ();
    pMessage1->setLogType (logType);
    pMessage1->setLogDescriptionType (logDescriptionType);
    pMessage1->setNumberOfLogsToAdd (BIST_NUMBER_OF_LOGS_TO_ADD);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage1);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void DistributedLogTestObjectManager::addAuditLog3UsingSendToWaveClusterTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    trace (TRACE_LEVEL_INFO, string ("Starting Add Audit Session Logout Using Send To Wave Cluster (") + BIST_NUMBER_OF_LOGS_TO_ADD + ") Test.");

    LogType             logType             = DISTRIBUTED_LOG_TYPE_AUDIT_LOG;
    LogDescriptionType  logDescriptionType  = DISTRIBUTED_LOG_SUB_TYPE_SESSION_LOGOUT;
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&DistributedLogTestObjectManager::addLogUsingSendToWaveClusterTestCallback), pDistributedLogTestContext);

    DistributedLogInvokeAddLogApiMessage *pMessage1 = new DistributedLogInvokeAddLogApiMessage ();
    pMessage1->setLogType (logType);
    pMessage1->setLogDescriptionType (logDescriptionType);
    pMessage1->setNumberOfLogsToAdd (BIST_NUMBER_OF_LOGS_TO_ADD);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage1);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void DistributedLogTestObjectManager::addManagedObjectLog1UsingSendToWaveClusterTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    trace (TRACE_LEVEL_INFO, string ("Starting Add Managed Object Log Using Send To Wave Cluster (") + BIST_NUMBER_OF_LOGS_TO_ADD + ") Test.");

    LogType             logType             = DISTRIBUTED_LOG_TYPE_MANAGED_OBJECT_LOG;
    LogDescriptionType  logDescriptionType  = DISTRIBUTED_LOG_SUB_TYPE_MANAGED_OBJECT_ENTRY;
    ObjectId            configInfoObjectId  = pDistributedLogTestContext->getManagedObjectId ();
    
    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&DistributedLogTestObjectManager::addLogUsingSendToWaveClusterTestCallback), pDistributedLogTestContext);

    DistributedLogInvokeAddLogApiMessage *pMessage1 = new DistributedLogInvokeAddLogApiMessage ();
    pMessage1->setLogType (logType);
    pMessage1->setLogDescriptionType (logDescriptionType);
    pMessage1->setManagedObjectId (configInfoObjectId);
    pMessage1->setNumberOfLogsToAdd (BIST_NUMBER_OF_LOGS_TO_ADD);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pMessage1);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void DistributedLogTestObjectManager::addLogUsingSendToWaveClusterTestCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    ResourceId                  sendToClusterStatus         = pWaveSendToClusterContext->getCompletionStatus ();
    DistributedLogTestContext  *pDistributedLogTestContext  = reinterpret_cast<DistributedLogTestContext *> (pWaveSendToClusterContext->getPCallerContext ());

    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;
 
    pDistributedLogTestContext->executeNextStep (sendToClusterStatus);
}

void DistributedLogTestObjectManager::restoreMaxAllowedLogEntriesTestStep (DistributedLogTestContext *pDistributedLogTestContext)
{
    UI64        numberOfLogEntries  = pDistributedLogTestContext->getOriginalMaxAllowedLogEntries ();
    ResourceId  status              = WAVE_MESSAGE_ERROR;

    trace (TRACE_LEVEL_INFO, string ("Starting Restore Maximum Allowed Log Entries (") + numberOfLogEntries + ") Test.");

    DistributedLogUpdateMaxLogEntriesMessage message (numberOfLogEntries);

    status = sendSynchronously (&message);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        ResourceId completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::restoreMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be processed. completionStatus = ") + completionStatus + ".");

            status = WAVE_MESSAGE_ERROR;
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogTestObjectManager::restoreMaxAllowedLogEntriesTestStep : Synchronously updating max allowed log entries failed to be sent. status = ") + status + ".");

        status = WAVE_MESSAGE_ERROR;
    }

    pDistributedLogTestContext->executeNextStep (status);
}

ResourceId DistributedLogTestObjectManager::queryForNumberOfLogEntries (UI64 &numberOfLogEntries)
{
    UI32    count               = 0;
            numberOfLogEntries  = 0;

    ResourceId status = querySynchronouslyForCount (DistributedLogEntryManagedObject::getClassName (), count);

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForNumberOfLogEntries : Query for count failed.  Reason : " + FrameworkToolKit::localize (status));
    }
    else
    {
        numberOfLogEntries = count;
    }

    return (status);
}

ResourceId DistributedLogTestObjectManager::queryForFirstLogId (UI64 &firstLogId)
{
    DistributedLogEntryManagedObject   *pDistributedLogEntryManagedObject   = NULL;
    ResourceId                          status                              = WAVE_MESSAGE_ERROR;
                                        firstLogId                          = 0;

    // Query distributed log for the first log entry

    WaveManagedObjectSynchronousQueryContext firstLogEntrySynchronousQueryContext   (DistributedLogEntryManagedObject::getClassName());
    firstLogEntrySynchronousQueryContext.addOrderField                              ("logId", true); // order by ascending logid's 
    firstLogEntrySynchronousQueryContext.addLimitForQueryResults                    (1);

    vector<WaveManagedObject *> *pFirstLogEntryResult = querySynchronously (&firstLogEntrySynchronousQueryContext);

    if (NULL != pFirstLogEntryResult)
    {
        UI32 numberOfLogEntries = pFirstLogEntryResult->size ();

        if (1 == numberOfLogEntries)
        {
            pDistributedLogEntryManagedObject = dynamic_cast<DistributedLogEntryManagedObject *> ((*pFirstLogEntryResult)[0]);
            firstLogId = pDistributedLogEntryManagedObject->getLogId ();

            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForFirstLogId : Query failed to get the first single DistributedLogEntryManagedObject.");

            status = WAVE_MESSAGE_ERROR;
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pFirstLogEntryResult);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForFirstLogId : Query failed for the first DistributedLogEntryManagedObject.");

        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId DistributedLogTestObjectManager::queryForLastLogId (UI64 &lastLogId)
{
    DistributedLogEntryManagedObject   *pDistributedLogEntryManagedObject   = NULL;
    ResourceId                          status                              = WAVE_MESSAGE_ERROR;
                                        lastLogId                           = 0;

    // Query distributed log for the last log entry

    WaveManagedObjectSynchronousQueryContext lastLogEntrySynchronousQueryContext    (DistributedLogEntryManagedObject::getClassName());
    lastLogEntrySynchronousQueryContext.addOrderField                               ("logId", false); // order by descending logid's
    lastLogEntrySynchronousQueryContext.addLimitForQueryResults                     (1);

    vector<WaveManagedObject *> *pLastLogEntryResult = querySynchronously (&lastLogEntrySynchronousQueryContext);

    if (NULL != pLastLogEntryResult)
    {
        UI32 numberOfResults = pLastLogEntryResult->size ();

        if (1 == numberOfResults)
        {
            pDistributedLogEntryManagedObject = dynamic_cast<DistributedLogEntryManagedObject *> ((*pLastLogEntryResult)[0]);
            lastLogId = pDistributedLogEntryManagedObject->getLogId ();
    
            status = WAVE_MESSAGE_SUCCESS;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForLastLogId : Query failed to get the last single DistributedLogEntryManagedObject.");

            status = WAVE_MESSAGE_ERROR;
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pLastLogEntryResult);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogTestObjectManager::queryForLastLogId : Query failed for the last DistributedLogEntryManagedObject.");

        status = WAVE_MESSAGE_ERROR;
    }

    return (status);
}

ResourceId DistributedLogTestObjectManager::queryForLogEntryInformation (UI64 &count, UI64 &firstLogId, UI64 &lastLogId)
{
    UI64        queriedCount        = 0;
    UI64        queriedFirstLogId   = 0;
    UI64        queriedLastLogId    = 0;
    ResourceId  overallStatus       = WAVE_MESSAGE_ERROR;

    ResourceId queryForNumberOfLogEntriesStatus = queryForNumberOfLogEntries (queriedCount);

    ResourceId queryForFistLogIdStatus = queryForFirstLogId (queriedFirstLogId);

    ResourceId queryForLastLogIdStatus = queryForLastLogId (queriedLastLogId);

    if (WAVE_MESSAGE_SUCCESS == queryForNumberOfLogEntriesStatus && WAVE_MESSAGE_SUCCESS == queryForFistLogIdStatus && WAVE_MESSAGE_SUCCESS == queryForLastLogIdStatus)
    {
        count           = queriedCount;
        firstLogId      = queriedFirstLogId;
        lastLogId       = queriedLastLogId;

        overallStatus   = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        overallStatus = WAVE_MESSAGE_ERROR;
    }

    return (overallStatus);
}

}
