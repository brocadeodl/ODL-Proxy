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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Cluster/MultiPartition/Global/WavePartitionManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Cluster/MultiPartition/Global/MultiPartitionTypes.h"
#include "Cluster/MultiPartition/Global/MultiPartitionObjectManager.h"
#include "Cluster/MultiPartition/Global/MultiPartitionGlobalCleanupAgent.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalMessages.h"
#include "Shell/ShellPrism.h"
#include "Shell/ShellDebug.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

namespace WaveNs
{

MultiPartitionObjectManager::MultiPartitionObjectManager ()
    : WaveObjectManager (getServiceName ())
{
    WavePartitionManagedObject   wavePartitionManagedObject(this);

    wavePartitionManagedObject.setupOrm ();

    addManagedClass (wavePartitionManagedObject.getClassName ());

    addOperationMap (MULTI_PARTITION_ADD_PARTITION, reinterpret_cast<PrismMessageHandler> (&MultiPartitionObjectManager::addPartitionMessageHandler));
    addOperationMap (MULTI_PARTITION_DELETE_PARTITION, reinterpret_cast<PrismMessageHandler> (&MultiPartitionObjectManager::deletePartitionMessageHandler));
    addOperationMap (MULTI_PARTITION_GET_OBJECTID_FOR_PARTITION_NAME, reinterpret_cast<PrismMessageHandler> (&MultiPartitionObjectManager::getObjectIdForPartitionNameMessageHandler));

//    addDebugFunction ((ShellCmdFunction) (&MultiPartitionObjectManager::distributedLogAddLogEntryDebugHandler), "addlogentry");
//    addDebugFunction ((ShellCmdFunction) (&MultiPartitionObjectManager::distributedLogUpdateMaxLogEntriesDebugHandler), "updatemaxlogentries");
}

MultiPartitionObjectManager::~MultiPartitionObjectManager ()
{
}

string MultiPartitionObjectManager::getServiceName ()
{
    return ("Multi Partition");
}

MultiPartitionObjectManager *MultiPartitionObjectManager::getInstance ()
{
    static MultiPartitionObjectManager *pMultiPartitionObjectManager = new MultiPartitionObjectManager ();

    WaveNs::prismAssert (NULL != pMultiPartitionObjectManager, __FILE__, __LINE__);

    return (pMultiPartitionObjectManager);
}

PrismServiceId MultiPartitionObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *MultiPartitionObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;
    
    switch (operationCode)
    {
        case MULTI_PARTITION_ADD_PARTITION :
            pPrismMessage = new MultiPartitionAddPartitionMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        case MULTI_PARTITION_DELETE_PARTITION :
            pPrismMessage = new MultiPartitionDeletePartitionMessage; 
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        case MULTI_PARTITION_GET_OBJECTID_FOR_PARTITION_NAME :
            pPrismMessage = new MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *MultiPartitionObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (WavePartitionManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "MultiPartitionObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new WavePartitionManagedObject (this);
    }
    else
    {
        trace(TRACE_LEVEL_ERROR, "MultiPartitionObjectManager::createManagedObjectInstance : Incorrect managed object for this object manager : " + managedClassName);
    }

    return pWaveManagedObject;
}

void MultiPartitionObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager:initialize : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void MultiPartitionObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    
    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::install : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void MultiPartitionObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::boot : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("BootReason : ") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason));

    if (WAVE_BOOT_FIRST_TIME_BOOT != bootReason)
    {
         // TODO: Futures.
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void MultiPartitionObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::haboot : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("BootReason : ") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}


void MultiPartitionObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason  = pFailoverAsynchronousContext->getfailoverReason ();

    trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover : [") + FrameworkToolKit::localizeToSourceCodeEnum (failoverReason) + string ("]"));

    do
    {
        vector<LocationId>  failedLocationIds       = pFailoverAsynchronousContext->getfailedLocationIds ();
        UI32                numberOfFailedLocations = failedLocationIds.size ();

        // prismAssert (0 != numberOfFailedLocations, __FILE__, __LINE__);

        if (0 == numberOfFailedLocations)
        {
            trace (TRACE_LEVEL_ERROR, "MultiPartitionObjectManager::failover : numberOfFailedLocations = 0");

            break;
        }

        if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED != failoverReason)
        {
            break;
        }

        vector<WaveManagedObject *> *pWavePartitionManagedObjects = querySynchronously (WavePartitionManagedObject::getClassName ());   // query ()
        prismAssert (NULL != pWavePartitionManagedObjects, __FILE__, __LINE__);

        UI32 numberOfWavePartitionManagedObjects = pWavePartitionManagedObjects->size ();

        if (0 == numberOfWavePartitionManagedObjects)
        {
            delete pWavePartitionManagedObjects;            // free (1)
            break;
        }

        vector<ObjectId>    failedLocationObjectIds;

        string failedLocationDebugString;

        for (UI32 i = 0; i < numberOfFailedLocations; i++)
        {
            LocationId failedLocationId = failedLocationIds[i];

            ObjectId objectIdForFailedLocation = FrameworkToolKit::getObjectIdForLocationId (failedLocationId);

            failedLocationObjectIds.push_back (objectIdForFailedLocation);

            failedLocationDebugString   = failedLocationDebugString + (UI32) failedLocationId;
            failedLocationDebugString  += " ";
        }

        trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover : failed LocationIds[ ") + failedLocationDebugString + string ("]"));

        vector<WavePartitionManagedObject *> pWavePartitionManagedObjectsForGarbageCollection;

        startTransaction ();    // [

        for (UI32 i = 0; i < numberOfWavePartitionManagedObjects; i++)
        {
            WavePartitionManagedObject *pWavePartitionManagedObject = dynamic_cast<WavePartitionManagedObject *> ((*pWavePartitionManagedObjects)[i]);
            prismAssert (NULL != pWavePartitionManagedObject, __FILE__, __LINE__);

            bool    failedLocationIsMappedToThisPartition   = false;
            UI32    remainingLocationsMappedToThisPartition = 0;

            for (UI32 j = 0; j < numberOfFailedLocations; j++)
            {
                ObjectId objectIdForFailedLocation = failedLocationObjectIds[j];

                if (false == pWavePartitionManagedObject->isWaveNodeMappedToPartition (objectIdForFailedLocation))
                {
                    continue;
                }

                failedLocationIsMappedToThisPartition = true;

                trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover : PartitionName [") + pWavePartitionManagedObject->getPartitionName () + string ("] : Removed Location [") + failedLocationIds[j] + string ("]"));

                remainingLocationsMappedToThisPartition = pWavePartitionManagedObject->removeWaveNodeFromPartition (objectIdForFailedLocation);

                if (0 == remainingLocationsMappedToThisPartition)
                {
                    break;
                }
            }

            if (true == failedLocationIsMappedToThisPartition)
            {
                if (0 == remainingLocationsMappedToThisPartition)
                {
                    trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover : PartitionName [") + pWavePartitionManagedObject->getPartitionName () + string ("] : Deleting Empty WavePartitionManagedObject."));

                    delete pWavePartitionManagedObject;                                                         // A -- Deleted from database, and memory.
                }
                else
                {
                    trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover : PartitionName [") + pWavePartitionManagedObject->getPartitionName () + string ("] : Updating WavePartitionManagedObject."));

                    updateWaveManagedObject (pWavePartitionManagedObject);
                    pWavePartitionManagedObjectsForGarbageCollection.push_back (pWavePartitionManagedObject);   // B -- Updated.
                }
            }
            else
            {
                pWavePartitionManagedObjectsForGarbageCollection.push_back (pWavePartitionManagedObject);       // C -- Unaffected.
            }
        }

        commitTransaction ();   // ]

        delete pWavePartitionManagedObjects;                //  free (2)

        for (UI32 i = 0; i < pWavePartitionManagedObjectsForGarbageCollection.size (); i++)
        {
            delete pWavePartitionManagedObjectsForGarbageCollection[i];     // B + C
        }

        trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::failover done."));
    }
    while (0);

    pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pFailoverAsynchronousContext->callback ();
}

void MultiPartitionObjectManager::addPartitionMessageHandler(MultiPartitionAddPartitionMessage *pMultiPartitionAddPartitionMessage)
{

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::validateInputsForAddPartition),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::createPartitionManagedObjectStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext = new MultiPartitionSynchronousLinearSequencerContext(pMultiPartitionAddPartitionMessage, 
                                                                                                        this, 
                                                                                                        sequencerSteps, 
                                                                                                        sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pMultiPartitionSynchronousLinearSequencerContext->execute ();
    return;
}

ResourceId MultiPartitionObjectManager::validateInputsForAddPartition(MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{
    MultiPartitionAddPartitionMessage *pMultiPartitionAddPartitionMessage = dynamic_cast<MultiPartitionAddPartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());

    prismAssert(NULL != pMultiPartitionAddPartitionMessage, __FILE__, __LINE__);
    string       partitionName              = pMultiPartitionAddPartitionMessage->getPartitionName ();
    LocationId   partitionLocationId = pMultiPartitionAddPartitionMessage->getPartitionLocationId();

    ResourceId   status  = WAVE_MESSAGE_ERROR;

    // Add a new entry into the log
    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::validateInputsForAddPartition called.");
    
    if (partitionName.empty())
    {
        return status;
    }

    ObjectId partitionLocationObjId = FrameworkToolKit::getObjectIdForLocationId(partitionLocationId);
    if (ObjectId::NullObjectId == partitionLocationObjId)
    {
        return status;
    }
    else 
    {
        pMultiPartitionSynchronousLinearSequencerContext->setWaveNodeObjectId(partitionLocationObjId);
    }

    // Check Existing.
   WaveManagedObjectSynchronousQueryContext  queryContext(WavePartitionManagedObject::getClassName());
   queryContext.addAndAttribute(new AttributeString(partitionName, "PartitionName"));

   vector<WaveManagedObject *> *pResult = querySynchronously(&queryContext); 
   if (NULL == pResult)
   {
       return status;
       // return error from Wave., potentially an assert.
   }

   UI32 numberOfResults = pResult->size(); 
   if (!numberOfResults)
   {
       // Need to create a new MO.
   } 
   else if (1 == numberOfResults)
   {
      WavePartitionManagedObject  *pWavePartitionManagedObject = dynamic_cast<WavePartitionManagedObject *>((*pResult)[0]);
      pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResult);
      // If this WaveNode is already in the partition's internal list, then error out.
      if (true == pWavePartitionManagedObject->isWaveNodeMappedToPartition(partitionLocationObjId))
      {
        delete pResult;  // cleanup.
        return status;
      }
      pMultiPartitionSynchronousLinearSequencerContext->setWavePartitionManagedObject(pWavePartitionManagedObject);
   }
   else 
   {
       trace(TRACE_LEVEL_FATAL, string("MultiPartitionObjectManager::validateInputsForAddPartition(), numberOfResults [") + numberOfResults + string("], PartitionName [") + partitionName + string("]") );
       pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResult);
       prismAssert(false, __FILE__, __LINE__);
   }

   delete pResult;  // cleanup.
   return (WAVE_MESSAGE_SUCCESS);
  
}

ResourceId MultiPartitionObjectManager::createPartitionManagedObjectStep(MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{
    MultiPartitionAddPartitionMessage *pMultiPartitionAddPartitionMessage = dynamic_cast<MultiPartitionAddPartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());

    prismAssert(NULL != pMultiPartitionAddPartitionMessage, __FILE__, __LINE__);
    string       partitionName  = pMultiPartitionAddPartitionMessage->getPartitionName ();

    ResourceId   status  = WAVE_MESSAGE_ERROR;

    WavePartitionManagedObject   *pWavePartitionManagedObject = pMultiPartitionSynchronousLinearSequencerContext->getWavePartitionManagedObject();
    if (NULL == pWavePartitionManagedObject)
    {
       pWavePartitionManagedObject = new WavePartitionManagedObject(this);
       prismAssert(NULL != pWavePartitionManagedObject, __FILE__, __LINE__);
       pWavePartitionManagedObject->setPartitionName(partitionName);

       pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pWavePartitionManagedObject);
    }
    else
    {
        updateWaveManagedObject(pWavePartitionManagedObject);
    }

    pMultiPartitionAddPartitionMessage->setCreatedWavePartitionManagedObjectId (pWavePartitionManagedObject->getObjectId ());
    
    ObjectId objId = pMultiPartitionSynchronousLinearSequencerContext->getWaveNodeObjectId();
    pWavePartitionManagedObject->addWaveNodeToPartition(objId);
    pMultiPartitionSynchronousLinearSequencerContext->setWavePartitionManagedObject(pWavePartitionManagedObject);
    status = WAVE_MESSAGE_SUCCESS;

    return status;
}

void MultiPartitionObjectManager::deletePartitionMessageHandler(MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage)
{
   // TODO: Delete sequencer/trigger failover code.
     PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::validateInputsForDeletePartition),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::triggerPartitionCleanupForGlocalServicesStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::deletePartitionManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext = new MultiPartitionSynchronousLinearSequencerContext(pMultiPartitionDeletePartitionMessage, 
                                                                                                        this, 
                                                                                                        sequencerSteps, 
                                                                                                        sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pMultiPartitionSynchronousLinearSequencerContext->execute ();
    return;
}

ResourceId MultiPartitionObjectManager::validateInputsForDeletePartition(MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{
    MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage = dynamic_cast<MultiPartitionDeletePartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());

    prismAssert(NULL != pMultiPartitionDeletePartitionMessage, __FILE__, __LINE__);
    string       partitionName              = pMultiPartitionDeletePartitionMessage->getPartitionName ();
    LocationId   partitionLocationId = pMultiPartitionDeletePartitionMessage->getPartitionLocationId();
    UI32         senderServiceId         = pMultiPartitionDeletePartitionMessage->getSenderServiceId();

    ResourceId   status  = WAVE_MESSAGE_ERROR;

    // Add a new entry into the log
    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::validateInputsForDeletePartition called.");
  
    if (true == pMultiPartitionDeletePartitionMessage->getIsPartialCleanup ())
    {
        ResourceId partialCleanupTag = pMultiPartitionDeletePartitionMessage->getPartialCleanupTag ();

        trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::validateInputsForDeletePartition : PartialCleanup : partialCleanupTag [") + FrameworkToolKit::localizeToSourceCodeEnum (partialCleanupTag) + string ("]"));
    }
    else
    {
        trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::validateInputsForDeletePartition : Non-partial Cleanup (delete)."));
    }

    if (partitionName.empty())
    {
        return status;
    }

    if (0 == senderServiceId)
    {
        return status;
    }

    ObjectId partitionLocationObjId = FrameworkToolKit::getObjectIdForLocationId(partitionLocationId);
    if (ObjectId::NullObjectId == partitionLocationObjId)
    {
        return status;
    }
    else 
    {
        pMultiPartitionSynchronousLinearSequencerContext->setWaveNodeObjectId(partitionLocationObjId);
    }

    // Check Existing.
   WaveManagedObjectSynchronousQueryContext  queryContext(WavePartitionManagedObject::getClassName());
   queryContext.addAndAttribute(new AttributeString(partitionName,    "PartitionName"));

   vector<WaveManagedObject *> *pResult = querySynchronously(&queryContext); 
   if (NULL == pResult)
   {
       return status;
       // return error from Wave., potentially an assert.
   }

   UI32 numberOfResults = pResult->size(); 
   if (!numberOfResults)
   {
       return status;
       // Handle Error. 
   } 
   else if (1 == numberOfResults)
   {
      WavePartitionManagedObject  *pWavePartitionManagedObject = dynamic_cast<WavePartitionManagedObject *>((*pResult)[0]);
      prismAssert(NULL != pWavePartitionManagedObject, __FILE__, __LINE__);
      // pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResult);

      // If this WaveNode is already in the partition's internal list, then proceed.
      if (true == pWavePartitionManagedObject->isWaveNodeMappedToPartition(partitionLocationObjId))
      {
          // Placeholder for any futures.
      } 
      else
      {
            pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResult);
            return status;
            // Handle error.
      }
      pMultiPartitionSynchronousLinearSequencerContext->setWavePartitionManagedObject(pWavePartitionManagedObject);
   }
   else 
   {
       trace(TRACE_LEVEL_FATAL, string("MultiPartitionObjectManager::validateInputsForDeletePartition(), numberOfResults [") + numberOfResults + string("], PartitionName [") + partitionName + string("]"));
       pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResult);
       prismAssert(false, __FILE__, __LINE__);
   }

   delete pResult;  // cleanup.
   return (WAVE_MESSAGE_SUCCESS);
  
}


ResourceId MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep(MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{
    // TODO: Trigger failover, and wait till it finishes. (Synchronous.). Sequencer's next step.
    // Now WaveNode from PartitionManagedObject's internal list.
    // if The Internal List becomes empty, then delete the PartitionManagedObject also.

    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep called.");
    MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage = dynamic_cast<MultiPartitionDeletePartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());

    prismAssert(NULL != pMultiPartitionDeletePartitionMessage, __FILE__, __LINE__);
    string       partitionName              = pMultiPartitionDeletePartitionMessage->getPartitionName ();
    LocationId   partitionLocationId        = pMultiPartitionDeletePartitionMessage->getPartitionLocationId ();
    ResourceId   status                  = WAVE_MESSAGE_SUCCESS;
    UI32         senderServiceId         = pMultiPartitionDeletePartitionMessage->getSenderServiceId();

    MultiPartitionDeleteLocalPartitionMessage  multiPartitionDeleteLocalPartitionMessage(partitionName);
    multiPartitionDeleteLocalPartitionMessage.setSenderServiceId (senderServiceId);
    
    WavePartitionManagedObject *pWavePartitionManagedObject = pMultiPartitionSynchronousLinearSequencerContext->getWavePartitionManagedObject();
    prismAssert(NULL != pWavePartitionManagedObject, __FILE__, __LINE__);
   
    multiPartitionDeleteLocalPartitionMessage.setOwnerPartitionManagedObjectId (pWavePartitionManagedObject->getObjectId());

    if (true == pMultiPartitionDeletePartitionMessage->getIsPartialCleanup ())
    {
        ResourceId partialCleanupTag = pMultiPartitionDeletePartitionMessage->getPartialCleanupTag ();

        multiPartitionDeleteLocalPartitionMessage.setPartialCleanupTag (partialCleanupTag);
    }

    ResourceId sendStatus = sendSynchronously (&multiPartitionDeleteLocalPartitionMessage, partitionLocationId);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = multiPartitionDeleteLocalPartitionMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            // successfully sent message to Local plugin to trigger cleanup. 
            trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep: for PARTITION [") + partitionName + string ("]"));
        }
        else
        {
            status = completionStatus;
            trace (TRACE_LEVEL_ERROR, string ("MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep: Failure in processing MultiPartitionDeletePartitionMessage, for vrf [") + partitionName + string ("], completionStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
        }
    }
    else
    {
        status = sendStatus;
        trace (TRACE_LEVEL_ERROR, string ("MultiPartitionObjectManager::triggerPartitionCleanupForLocalServicesStep: Failure in procecessing MultiPartitionDeletePartitionMessage, for vrf [") + partitionName + string ("], sendStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
    }

    return (status);
}

ResourceId MultiPartitionObjectManager::triggerPartitionCleanupForGlocalServicesStep (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::triggerPartitionCleanupForGlocalServicesStep  called. ");

    MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage = dynamic_cast<MultiPartitionDeletePartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pMultiPartitionDeletePartitionMessage, __FILE__, __LINE__);

    string   partitionName     = pMultiPartitionDeletePartitionMessage->getPartitionName ();
    UI32     senderServiceId   = pMultiPartitionDeletePartitionMessage->getSenderServiceId();

    WavePartitionManagedObject *pWavePartitionManagedObject = pMultiPartitionSynchronousLinearSequencerContext->getWavePartitionManagedObject();
    prismAssert(NULL != pWavePartitionManagedObject, __FILE__, __LINE__);

    ObjectId ownerPartitionObjectId = pWavePartitionManagedObject->getObjectId();
   
    MultiPartitionGlobalCleanupAgent  *pMultiPartitionGlobalCleanupAgent  = new MultiPartitionGlobalCleanupAgent (this, partitionName, senderServiceId , ownerPartitionObjectId);
    prismAssert(NULL != pMultiPartitionGlobalCleanupAgent, __FILE__ , __LINE__);

    if (true == pMultiPartitionDeletePartitionMessage->getIsPartialCleanup ())
    {
        ResourceId partialCleanupTag = pMultiPartitionDeletePartitionMessage->getPartialCleanupTag ();

        pMultiPartitionGlobalCleanupAgent->setPartialCleanupTag (partialCleanupTag);
    }

    tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionObjectManager::triggerPartitionCleanupForGlocalServicesStep: Before GlobalCleanupAgent is called. " );
    status = pMultiPartitionGlobalCleanupAgent->execute();
    tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionObjectManager::triggerPartitionCleanupForGlocalServicesStep: After  GlobalCleanupAgent is called. " );

    delete pMultiPartitionGlobalCleanupAgent;

    return (status);     
}

ResourceId MultiPartitionObjectManager::deletePartitionManagedObject (MultiPartitionSynchronousLinearSequencerContext *pMultiPartitionSynchronousLinearSequencerContext)
{

    WavePartitionManagedObject *pWavePartitionManagedObject = pMultiPartitionSynchronousLinearSequencerContext->getWavePartitionManagedObject();
    prismAssert(NULL != pWavePartitionManagedObject, __FILE__, __LINE__);

    MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage = dynamic_cast<MultiPartitionDeletePartitionMessage *> (pMultiPartitionSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pMultiPartitionDeletePartitionMessage, __FILE__, __LINE__);

    if (true == pMultiPartitionDeletePartitionMessage->getIsPartialCleanup ())
    {
        ResourceId partialCleanupTag = pMultiPartitionDeletePartitionMessage->getPartialCleanupTag ();

        trace (TRACE_LEVEL_INFO, string ("MultiPartitionObjectManager::deletePartitionManagedObject : PartialCleanup : partialCleanupTag [") + FrameworkToolKit::localizeToSourceCodeEnum (partialCleanupTag) + string ("]. Not deleting PartitionManagedObject."));

        pMultiPartitionSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pWavePartitionManagedObject);

        return WAVE_MESSAGE_SUCCESS;
    }

    ObjectId partitionLocationObjId =  pMultiPartitionSynchronousLinearSequencerContext->getWaveNodeObjectId();
    trace (TRACE_LEVEL_INFO, "MultiPartitionObjectManager::deletePartitionManagedObject called.");

    UI32 nPartitionLocationCount = pWavePartitionManagedObject->removeWaveNodeFromPartition(partitionLocationObjId);
    if (0 == nPartitionLocationCount)
    {
        delete pWavePartitionManagedObject;
    }
    else
    {
        updateWaveManagedObject(pWavePartitionManagedObject);
    }

    return (WAVE_MESSAGE_SUCCESS);
}

void MultiPartitionObjectManager::getObjectIdForPartitionNameMessageHandler (MultiPartitionObjectManagerGetObjectIdForPartitionNameMessage *pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage)
{
    ResourceId  completionStatus    = WAVE_MESSAGE_ERROR;
    string      partitionName       = pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage->getPartitionName ();

    if (false == partitionName.empty ())
    {
        ObjectId partitionObjectId = getObjectIdForPartitionName (partitionName);

        if (ObjectId::NullObjectId != partitionObjectId)
        {
            completionStatus = WAVE_MESSAGE_SUCCESS;

            pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage->setPartitionObjectId (partitionObjectId);
        }
    }

    pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage->setCompletionStatus (completionStatus);
    reply (pMultiPartitionObjectManagerGetObjectIdForPartitionNameMessage);
}

ObjectId MultiPartitionObjectManager::getObjectIdForPartitionName (const string &partitionName)
{
    ObjectId partitionObjectId;

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (WavePartitionManagedObject::getClassName ());
    waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeString (partitionName, "PartitionName"));

    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&waveManagedObjectSynchronousQueryContext);

    if (NULL == pWaveManagedObjects)
    {
        return partitionObjectId;
    }

    UI32 numberOfPartitionObjects = pWaveManagedObjects->size ();

    if (1 == numberOfPartitionObjects)
    {
        WavePartitionManagedObject *pWavePartitionManagedObject = dynamic_cast<WavePartitionManagedObject*> ((*pWaveManagedObjects)[0]);
        prismAssert (NULL != pWavePartitionManagedObject, __FILE__, __LINE__);

        partitionObjectId = pWavePartitionManagedObject->getObjectId ();
    }

    if (1 < numberOfPartitionObjects)
    {
        trace (TRACE_LEVEL_FATAL, string ("MultiPartitionObjectManager::getObjectIdForPartitionName : [") + numberOfPartitionObjects + string ("] found for partitionName [") + partitionName + string ("]"));
        prismAssert (false, __FILE__, __LINE__);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveManagedObjects);

    return partitionObjectId;
}

}
