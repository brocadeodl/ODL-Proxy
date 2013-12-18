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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#include "Framework/DistributedLog/DistributedLogObjectManager.h"
#include "Framework/DistributedLog/DistributedLogConfigurationManagedObject.h"
#include "Framework/DistributedLog/DistributedLogEntryManagedObject.h"
#include "Framework/DistributedLog/DistributedLogAddLogEntryMessage.h"
#include "Framework/DistributedLog/DistributedLogUpdateLogIdMessage.h"
#include "Framework/DistributedLog/DistributedLogUpdateMaxLogEntriesMessage.h"
#include "Framework/DistributedLog/DistributedLogTypes.h"
#include "Framework/DistributedLog/DistributedLogSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Shell/ShellDebug.h"
#include "Shell/ShellPrism.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

namespace WaveNs
{
    static const UI64 MAX_DEFAULT_LOG_ENTRIES = 100000;

DistributedLogObjectManager::DistributedLogObjectManager ()
    : WaveObjectManager (getServiceName ()),
    m_maxLogEntries                             (0),
    m_firstLogId                                (0),
    m_nextLogId                                 (0)
{
    DistributedLogConfigurationManagedObject    distributedLogConfigurationManagedObject (this);
    DistributedLogEntryManagedObject            distributedLogEntryManagedObject         (this);

    distributedLogConfigurationManagedObject.setupOrm ();
    distributedLogEntryManagedObject.setupOrm ();

    addManagedClass (DistributedLogConfigurationManagedObject::getClassName ());
    addManagedClass (DistributedLogEntryManagedObject::getClassName ());

    addOperationMap (DISTRIBUTED_LOG_ADD_LOG_ENTRY, reinterpret_cast<PrismMessageHandler> (&DistributedLogObjectManager::distributedLogAddLogEntryMessageHandler));
    addOperationMap (DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES, reinterpret_cast<PrismMessageHandler> (&DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesMessageHandler));
    addOperationMap (DISTRIBUTED_LOG_UPDATE_IN_MEMORY_LOG_ID, reinterpret_cast<PrismMessageHandler> (&DistributedLogObjectManager::distributedLogUpdateInMemoryLogIdMessageHandler));

    addDebugFunction ((ShellCmdFunction) (&DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler), "addlogentry");
    addDebugFunction ((ShellCmdFunction) (&DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler), "updatemaxlogentries");
}

DistributedLogObjectManager::~DistributedLogObjectManager ()
{
}

string DistributedLogObjectManager::getServiceName ()
{
    return ("Distributed Log");
}

DistributedLogObjectManager *DistributedLogObjectManager::getInstance ()
{
    static DistributedLogObjectManager *pDistributedLogObjectManager = new DistributedLogObjectManager ();

    WaveNs::prismAssert (NULL != pDistributedLogObjectManager, __FILE__, __LINE__);

    return (pDistributedLogObjectManager);
}

PrismServiceId DistributedLogObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *DistributedLogObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case DISTRIBUTED_LOG_ADD_LOG_ENTRY :
            pPrismMessage = new DistributedLogAddLogEntryMessage ();
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        case DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES :
            pPrismMessage = new DistributedLogUpdateMaxLogEntriesMessage ();
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *DistributedLogObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (DistributedLogConfigurationManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new DistributedLogConfigurationManagedObject (this);
    }
    else if (DistributedLogEntryManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new DistributedLogEntryManagedObject (this);
    }
    else
    {
        trace(TRACE_LEVEL_ERROR, "DistributedLogObjectManager::createManagedObjectInstance : Incorrect managed object for this object manager : " + managedClassName);
    }

    return pWaveManagedObject;
}

void DistributedLogObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager:initialize : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_DEBUG, string ("DistributedLogObjectManager::initialize : BootReason : ") + FrameworkToolKit::localize (bootReason));

    if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
    {
        m_maxLogEntries = MAX_DEFAULT_LOG_ENTRIES;
        m_firstLogId    = 0;
        m_nextLogId     = 0;

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();

        return;
    }
    else
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        pDistributedLogSynchronousLinearSequencerContext->execute ();

        return;
    }
}

void DistributedLogObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::install : Entering ...");

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogInstallDefaultConfigurationMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogSynchronousLinearSequencerContext->execute ();
}

void DistributedLogObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::haboot : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_DEBUG, string ("DistributedLogObjectManager::haboot : BootReason : ") + FrameworkToolKit::localize (bootReason));

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogSynchronousLinearSequencerContext->execute ();
}

ResourceId DistributedLogObjectManager::distributedLogInstallDefaultConfigurationMOStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    // The distributed log configuration managed object is queried to ensure we only create a single instance during this install phase.

    vector<WaveManagedObject *> *pResults = querySynchronously (DistributedLogConfigurationManagedObject::getClassName ());
    
    if (NULL != pResults)
    {
        if (0 == pResults->size ())
        {
            // Create a new configuration MO

            DistributedLogConfigurationManagedObject *pDistributedLogConfigurationManagedObject = new DistributedLogConfigurationManagedObject (this);

            prismAssert (NULL != pDistributedLogConfigurationManagedObject, __FILE__, __LINE__);

            pDistributedLogConfigurationManagedObject->setMaxLogEntries (m_maxLogEntries);
            pDistributedLogConfigurationManagedObject->setFirstLogId    (m_firstLogId);
            pDistributedLogConfigurationManagedObject->setNextLogId     (m_nextLogId);

            pDistributedLogSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pDistributedLogConfigurationManagedObject);
        }

        pDistributedLogSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pResults);

        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "DistributedLogObjectManager::distributedLogInstallDefaultConfigurationMOStep : Query of configuration managed object failed.");

        prismAssert (false, __FILE__, __LINE__);
    }

    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    vector<WaveManagedObject *> *pResults = querySynchronously (DistributedLogConfigurationManagedObject::getClassName ());

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (1 == numberOfResults)
        {
            DistributedLogConfigurationManagedObject *pDistributedLogConfigurationManagedObject = dynamic_cast<DistributedLogConfigurationManagedObject *> ((*pResults)[0]);

            prismAssert (NULL != pDistributedLogConfigurationManagedObject, __FILE__, __LINE__);

            m_maxLogEntries = pDistributedLogConfigurationManagedObject->getMaxLogEntries ();
        }
        else if (0 == numberOfResults)
        {
            m_maxLogEntries = MAX_DEFAULT_LOG_ENTRIES;

            trace (TRACE_LEVEL_INFO, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep : Query for the DistributedLogConfigurationManagedObject returned 0 results.  Setting the max log entries to default : ") + m_maxLogEntries);
        }
        else
        {
            m_maxLogEntries = MAX_DEFAULT_LOG_ENTRIES;

            trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep : Query for the DistributedLogConfigurationManagedObject returned more than one results.  This could be due to DB corrution.  Setting the max log entries to default : ") + m_maxLogEntries);
        }
    }
    else
    {
        m_maxLogEntries = MAX_DEFAULT_LOG_ENTRIES;

        trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromConfigurationMOStep : Query for the DistributedLogConfigurationManagedObject failed.  Setting the max log entries to default : ") + m_maxLogEntries);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    vector<WaveManagedObject *>    *pResults        = NULL;
    UI32                            numberOfResults = 0;
    ResourceId                      status          = WAVE_MESSAGE_SUCCESS;

    // Get the first and last log ids by examining the distributed log entries.
    //
    // First get the first id.

    WaveManagedObjectSynchronousQueryContext queryContextForFirstLogId (DistributedLogEntryManagedObject::getClassName ());

    queryContextForFirstLogId.addOrderField ("logId", true);
    queryContextForFirstLogId.addLimitForQueryResults (1);

    pResults = querySynchronously (&queryContextForFirstLogId);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    numberOfResults = pResults->size ();

    prismAssert (1 >= numberOfResults, __FILE__, __LINE__);

    if (1 == numberOfResults)
    {
        DistributedLogEntryManagedObject *pDistributedLogEntryManagedObject = dynamic_cast<DistributedLogEntryManagedObject *> ((*pResults)[0]);

        prismAssert (NULL != pDistributedLogEntryManagedObject, __FILE__, __LINE__);

        m_firstLogId = pDistributedLogEntryManagedObject->getLogId ();

        trace (TRACE_LEVEL_INFO, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep : First Log Id : ") + m_firstLogId);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep : Query for the first logId returned 0 results.  In memory First Log Id : ") + m_firstLogId);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    // Next get the next id.

    WaveManagedObjectSynchronousQueryContext queryContextForLastLogId (DistributedLogEntryManagedObject::getClassName ());

    queryContextForLastLogId.addOrderField ("logId", false);
    queryContextForLastLogId.addLimitForQueryResults (1);

    pResults = querySynchronously (&queryContextForLastLogId);

    prismAssert (NULL != pResults, __FILE__, __LINE__);

    numberOfResults = pResults->size ();

    prismAssert (1 >= numberOfResults, __FILE__, __LINE__);

    if (1 == numberOfResults)
    {
        DistributedLogEntryManagedObject *pDistributedLogEntryManagedObject = dynamic_cast<DistributedLogEntryManagedObject *> ((*pResults)[0]);

        prismAssert (NULL != pDistributedLogEntryManagedObject, __FILE__, __LINE__);

        m_nextLogId = pDistributedLogEntryManagedObject->getLogId ();
        m_nextLogId++;

        trace (TRACE_LEVEL_INFO, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep : Next Log Id : ") + m_nextLogId);
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep : Query for the last logId returned 0 results.  In memory Next Log Id : ") + m_nextLogId);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);

    return (status);
}

void DistributedLogObjectManager::dbInconsistencyCheck (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    //ResourceId  status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "DistributedLogObjectManager::dbInconsistencyCheck : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
    trace (TRACE_LEVEL_INFO, string ("DistributedLogObjectManager::dbInconsistencyCheck : BootReason : ") + FrameworkToolKit::localize (bootReason));

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (DistributedLogConfigurationManagedObject::getClassName ());
    //status = FrameworkToolKit::checkForExistenceOfOneMO (&synchronousQueryContext);
    /*
    vector<WaveManagedObject *> *pResults = querySynchronously (DistributedLogConfigurationManagedObject::getClassName ());
    UI32 i = 0; 

    if (0 == pResults->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::dbInconsistencyCheck : Zero entries found in DistributedLogConfigurationManagedObject.")); 
        status = WAVE_MESSAGE_ERROR;
    }
    else if (2 == pResults->size ())
    {
        ObjectId object1 = ((*pResults)[0])->getObjectId ();
        ObjectId object2 = ((*pResults)[1])->getObjectId ();
        if (object1 == object2)
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::dbInconsistencyCheck : Duplicate entries found in DistributedLogConfigurationManagedObject. The ObjectIds are ") + object1.toString () + " and " + object2.toString ());
            status = WAVE_MESSAGE_ERROR;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::dbInconsistencyCheck : Two entries found in DistributedLogConfigurationManagedObject."));
            prismAssert (false, __FILE__, __LINE__);
        }

    }
    else if (2 < pResults->size ())
    {
        trace (TRACE_LEVEL_ERROR, string ("DistributedLogObjectManager::dbInconsistencyCheck : Multiple entries found in DistributedLogConfigurationManagedObject."));
        prismAssert (false, __FILE__, __LINE__);
    }
    for (i = 0; i < pResults->size (); i++)
    {
        delete (*pResults)[i];
    }

    pResults->clear ();
    delete pResults;
    */

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (FrameworkToolKit::checkForExistenceOfOneMO (&synchronousQueryContext, DistributedLogConfigurationManagedObject::getClassName ()));
    pWaveAsynchronousContextForBootPhases->callback ();
}


void DistributedLogObjectManager::distributedLogAddLogEntryMessageHandler (DistributedLogAddLogEntryMessage *pDistributedLogAddLogEntryMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogSetupADelayedCommitTransactionIfRequiredStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogAddLogEntryStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateFirstAndNextLogIdStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    pDistributedLogAddLogEntryMessage->setIsConfigurationChange (false);

    DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pDistributedLogAddLogEntryMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogSynchronousLinearSequencerContext->execute ();
}

void DistributedLogObjectManager::distributedLogUpdateInMemoryLogIdMessageHandler (DistributedLogUpdateLogIdMessage *pDistributedLogUpdateLogIdMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateInMemoryObjectsFromEntriesMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pDistributedLogUpdateLogIdMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogSynchronousLinearSequencerContext->execute ();
}

void DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesMessageHandler (DistributedLogUpdateMaxLogEntriesMessage *pDistributedLogUpdateMaxLogEntriesMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogRemoveLogEntriesIfRequiredStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedLogObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    pDistributedLogUpdateMaxLogEntriesMessage->setIsConfigurationChange (false);

    DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext = new DistributedLogSynchronousLinearSequencerContext (pDistributedLogUpdateMaxLogEntriesMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pDistributedLogSynchronousLinearSequencerContext->execute ();
}

ResourceId DistributedLogObjectManager::distributedLogSetupADelayedCommitTransactionIfRequiredStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    DistributedLogAddLogEntryMessage *pDistributedLogAddLogEntryMessage = dynamic_cast<DistributedLogAddLogEntryMessage *> (pDistributedLogSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pDistributedLogAddLogEntryMessage, __FILE__, __LINE__);

    ResourceId                        logDescriptionType                = pDistributedLogAddLogEntryMessage->getLogDescriptionType ();
    ResourceId                        status                            = WAVE_MESSAGE_SUCCESS;

    // Enabling delayed commit transaction for adding certain types of distributed log entries.  The commit transaction will not be immediately committed into the database when the commit transaction step is called.  Rather it will be queued up in Persistence OM and committed by being piggy-backed on any subsequent executed commit transaction.  This is to improve performance for distributed logging.  

    if (DISTRIBUTED_LOG_SUB_TYPE_COMMAND_CONFIGURATION == logDescriptionType)
    {
        pDistributedLogSynchronousLinearSequencerContext->setIsADelayedCommitTransaction (true);
    }
    else
    {
        pDistributedLogSynchronousLinearSequencerContext->setIsADelayedCommitTransaction (false);
    }

    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogAddLogEntryStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    DistributedLogAddLogEntryMessage *pDistributedLogAddLogEntryMessage = dynamic_cast<DistributedLogAddLogEntryMessage *> (pDistributedLogSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pDistributedLogAddLogEntryMessage, __FILE__, __LINE__);

    ResourceId                        logType                           = pDistributedLogAddLogEntryMessage->getLogType ();
    ResourceId                        logDescriptionType                = pDistributedLogAddLogEntryMessage->getLogDescriptionType ();
    DateTime                          dateTimeStamp                     = pDistributedLogAddLogEntryMessage->getDateTimeStamp ();
    string                            username                          = pDistributedLogAddLogEntryMessage->getUsername ();
    PrismServiceId                    prismServiceId                    = pDistributedLogAddLogEntryMessage->getPrismServiceId ();
    string                            description                       = pDistributedLogAddLogEntryMessage->getDescription ();
    ObjectId                          waveNodeObjectId                  = pDistributedLogAddLogEntryMessage->getWaveNodeObjectId ();
    ObjectId                          managedObjectId                   = pDistributedLogAddLogEntryMessage->getManagedObjectId ();

    UI64                              numberOfLogEntries                = getNumberOfLogEntries ();

    DistributedLogEntryManagedObject *pDistributedLogEntryManagedObject = NULL;
    ResourceId                        status                            = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEBUG, string ("DistributedLogObjectManager::distributedLogAddLogEntryStep : numberOfLogEntries : ") + numberOfLogEntries + ", maxLogEntries : " + m_maxLogEntries);

    if (numberOfLogEntries < m_maxLogEntries)
    {
        // Add a new entry into the log

        trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::distributedLogAddLogEntryStep : Adding log entry");

        pDistributedLogEntryManagedObject = new DistributedLogEntryManagedObject (this);

        prismAssert (NULL != pDistributedLogEntryManagedObject, __FILE__, __LINE__);

        pDistributedLogEntryManagedObject->setLogId (m_nextLogId);
        pDistributedLogEntryManagedObject->setLogType (logType);
        pDistributedLogEntryManagedObject->setLogDescriptionType (logDescriptionType);
        pDistributedLogEntryManagedObject->setDateTimeStamp (dateTimeStamp);
        pDistributedLogEntryManagedObject->setPrismServiceId (prismServiceId);
        pDistributedLogEntryManagedObject->setUsername (username);
        pDistributedLogEntryManagedObject->setDescription (description);
        pDistributedLogEntryManagedObject->setWaveNodeObjectId (waveNodeObjectId);
        pDistributedLogEntryManagedObject->setManagedObjectId (managedObjectId);

        pDistributedLogSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pDistributedLogEntryManagedObject);
    }
    else
    {
        // Query for the First logId (oldest) log entry which will be replaced.

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DistributedLogEntryManagedObject::getClassName ());

        synchronousQueryContext.addAndAttribute (new AttributeUI64 (m_firstLogId, "logId"));

        vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

        UI64 numberOfResults = 0;
    
        if (NULL != pResults)
        {   
            numberOfResults = pResults->size ();

            if (1 == numberOfResults)
            {
                // Rotate circular buffer and replace the oldest log entry

                trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::distributedLogAddLogEntryStep : Adding rotated log entry");

                pDistributedLogEntryManagedObject = dynamic_cast<DistributedLogEntryManagedObject *> ((*pResults)[0]);

                prismAssert (NULL != pDistributedLogEntryManagedObject, __FILE__, __LINE__);

                updateWaveManagedObject (pDistributedLogEntryManagedObject);

                pDistributedLogEntryManagedObject->setLogId (m_nextLogId);
                pDistributedLogEntryManagedObject->setLogType (logType);
                pDistributedLogEntryManagedObject->setLogDescriptionType (logDescriptionType);
                pDistributedLogEntryManagedObject->setDateTimeStamp (dateTimeStamp);
                pDistributedLogEntryManagedObject->setPrismServiceId (prismServiceId);
                pDistributedLogEntryManagedObject->setUsername (username);
                pDistributedLogEntryManagedObject->setDescription (description);
                pDistributedLogEntryManagedObject->setWaveNodeObjectId (waveNodeObjectId);
                pDistributedLogEntryManagedObject->setManagedObjectId (managedObjectId);

                pDistributedLogSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pResults);
            }
            else
            {
                trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryStep : no MO found");
                
                status = WAVE_MESSAGE_ERROR;
            }

            delete pResults;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryStep : Query unsuccessful");

            status = WAVE_MESSAGE_ERROR;
        }
    }

    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogUpdateFirstAndNextLogIdStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::distributedLogUpdateFirstAndNextLogIdStep : Entering...");

    // Update cached logIds

    if (isLogFull ())
    {
        ++m_firstLogId;
    }

    ++m_nextLogId;
    
    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogRemoveLogEntriesIfRequiredStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    DistributedLogUpdateMaxLogEntriesMessage *pDistributedLogUpdateMaxLogEntriesMessage = dynamic_cast<DistributedLogUpdateMaxLogEntriesMessage *> (pDistributedLogSynchronousLinearSequencerContext->getPPrismMessage ());
    UI64                                      newMaxLogEntries                          = pDistributedLogUpdateMaxLogEntriesMessage->getMaxLogEntries ();
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::distributedLogRemoveLogEntriesIfRequiredStep : Entering...");

    if (0 < newMaxLogEntries)
    {
        trace (TRACE_LEVEL_INFO, string ("DistributedLogObjectManager::distributedLogRemoveLogEntriesIfRequiredStep : Updating maximum log entries from ") + m_maxLogEntries + " to " + newMaxLogEntries + ".");

        UI64 numberOfLogEntries         = getNumberOfLogEntries ();
        UI64 numberOfLogEntriesToRemove = 0;
        UI64 upperBoundLogId            = 0;
        UI64 firstLogId                 = m_firstLogId;

        if (newMaxLogEntries < numberOfLogEntries)
        {
            numberOfLogEntriesToRemove  = numberOfLogEntries - newMaxLogEntries;
            upperBoundLogId             = m_firstLogId + numberOfLogEntriesToRemove;
            
            WaveManagedObjectSynchronousQueryContextForDeletion *synchronousQueryContextForDeletion = new WaveManagedObjectSynchronousQueryContextForDeletion (DistributedLogEntryManagedObject::getClassName());

            synchronousQueryContextForDeletion->addAndAttribute (new AttributeUI64 (firstLogId, "logId"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_GREATER_THAN_OR_EQUAL_TO);
            synchronousQueryContextForDeletion->addAndAttribute (new AttributeUI64 (upperBoundLogId, "logId"), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LESS_THAN);

            deleteMultipleWaveManagedObjects (synchronousQueryContextForDeletion);
        }

        // firstLogId database and cache will be updated in next step

        pDistributedLogSynchronousLinearSequencerContext->setNumberOfLogEntries (numberOfLogEntriesToRemove);
    }
    else 
    {
        trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogRemoveLogEntriesIfRequiredStep : Invalid size given.  Maximum log entries must be greater than zero.");
        
        status = WAVE_MESSAGE_ERROR;
    }
        
    return (status);
} 

ResourceId DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesStep (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext)
{
    DistributedLogUpdateMaxLogEntriesMessage *pDistributedLogUpdateMaxLogEntriesMessage = dynamic_cast<DistributedLogUpdateMaxLogEntriesMessage *> (pDistributedLogSynchronousLinearSequencerContext->getPPrismMessage ());
    UI64                                      newMaxLogEntries                          = pDistributedLogUpdateMaxLogEntriesMessage->getMaxLogEntries ();
    ResourceId                                status                                    = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_DEVEL, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesStep : Entering...");

    // Update cached maxLogEntries

    m_maxLogEntries = newMaxLogEntries;

    // Update cached firstLogId if log entries were removed from previous step

    UI64 numberOfEntriesRemoved = pDistributedLogSynchronousLinearSequencerContext->getNumberOfLogEntries ();

    if (0 < numberOfEntriesRemoved)
    {
        m_firstLogId = m_firstLogId + numberOfEntriesRemoved;
    }

    // Query for DistributedLogConfigurationManagedObject for an update

    vector<WaveManagedObject *> *pResults = querySynchronously (DistributedLogConfigurationManagedObject::getClassName ());

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (numberOfResults == 1)
        {
            DistributedLogConfigurationManagedObject *pDistributedLogConfigurationManagedObject = dynamic_cast<DistributedLogConfigurationManagedObject *> ((*pResults)[0]);
            prismAssert (NULL != pDistributedLogConfigurationManagedObject, __FILE__, __LINE__);

            updateWaveManagedObject (pDistributedLogConfigurationManagedObject);

            pDistributedLogConfigurationManagedObject->setMaxLogEntries (m_maxLogEntries);

            // First and Last Log Ids are now determined from the log entries managed object.  Saving first and next log ids in the log configuration managed object will be deprecated.

            pDistributedLogConfigurationManagedObject->setFirstLogId (m_firstLogId);
            pDistributedLogConfigurationManagedObject->setNextLogId (m_nextLogId);

            pDistributedLogSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pResults);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesStep : no MO found");

            status = WAVE_MESSAGE_ERROR;
        }

        delete pResults;
    }

    return (status);
}

ResourceId DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler (UI32 argc, vector<string> argv)
{
    UI32 numberOfArguments = argv.size ();

    if (3 != numberOfArguments)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : USAGE :");
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : addlogentry <description> <username>");

        return (0);
    }

    WaveNs::trace (TRACE_LEVEL_INFO, "DistributedLogObjectManager::distributeLogAddLogEntryDebugHandler : Received input to add log entry with description|username : " + argv[1] + string ("|") + argv[2]);

    DistributedLogAddLogEntryMessage message (DISTRIBUTED_LOG_TYPE_GENERIC_LOG, DISTRIBUTED_LOG_SUB_TYPE_GENERIC_ENTRY, argv[1], argv[2]);
    ResourceId                       status           = WAVE_MESSAGE_ERROR;
    ResourceId                       completionStatus = WAVE_MESSAGE_ERROR;

    status = ShellPrism::shellSendSynchronously (&message);

    WaveNs::trace (TRACE_LEVEL_INFO, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : Returned from shellSendSynchrounously.");

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : Successfully created log entry description.");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : Failed to create log entry description.  Status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogAddLogEntryDebugHandler : Could not send message to Distributed Log Service.  Status : " + FrameworkToolKit::localize (status));
    }

    return (0);
}

ResourceId DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler (UI32 argc, vector<string> argv)
{
    UI32 numberOfArguments = argv.size ();

    if (2 != numberOfArguments)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : USAGE :");
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : updatemaxlogentries <max log entries>");

        return (0);
    }

    WaveNs::trace (TRACE_LEVEL_INFO, "DistributedLogObjectManager::distributeLogUpdateMaxLogEntriesDebugHandler : Received input to update max log entries : " + argv[1]);

    DistributedLogUpdateMaxLogEntriesMessage message            ((UI32) atoi(argv[1].c_str()));
    ResourceId                               status           = WAVE_MESSAGE_ERROR;
    ResourceId                               completionStatus = WAVE_MESSAGE_ERROR;

    status = ShellPrism::shellSendSynchronously (&message);

    WaveNs::trace (TRACE_LEVEL_INFO, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : Returned from shellSendSynchrounously.");

    if (WAVE_MESSAGE_SUCCESS == status)
    {
        completionStatus = message.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : Successfully updated maxium log entries.");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : Failed to update maximum log entries.  Status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "DistributedLogObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler : Could not send message to Distributed Log Service.  Status : " + FrameworkToolKit::localize (status));
    }

    return (0);
}

UI64 DistributedLogObjectManager::getNumberOfLogEntries () const
{
    return (m_nextLogId - m_firstLogId);
}

bool DistributedLogObjectManager::isLogEmpty () const
{
    UI64 numberOfEntries = getNumberOfLogEntries ();
    bool isEmpty         = true;

    if (0 == numberOfEntries)
    {
        isEmpty = true;
    }
    else
    {
        isEmpty = false;
    }

    return (isEmpty);
}

bool DistributedLogObjectManager::isLogFull () const
{
    UI64 numberOfEntries = getNumberOfLogEntries ();
    bool isFull          = false;

    if (false == isLogEmpty () && numberOfEntries >= m_maxLogEntries)
    {
        isFull = true;
    }
    else
    {
        isFull = false;
    }

    return (isFull);
}

}
