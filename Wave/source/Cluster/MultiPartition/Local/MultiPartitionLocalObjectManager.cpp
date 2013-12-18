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

/*********************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.    *
 *   All rights reserved.                                            *
 *   Author : Jayaraman Iyer                                         *
 *********************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalObjectManager.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalMessages.h"
#include "Cluster/MultiPartition/Local/MultiPartitionCleanupAgent.h"
#include "Cluster/MultiPartition/Local/MultiPartitionLocalTypes.h"
#include "WaveResourceIds.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace WaveNs
{

    MultiPartitionLocalObjectManager::MultiPartitionLocalObjectManager ()  : WaveLocalObjectManager  (getServiceName ())
    {
        addOperationMap (MULTI_PARTITION_DELETE_LOCAL_PARTITION_INSTANCE,  reinterpret_cast<PrismMessageHandler> (&MultiPartitionLocalObjectManager::tenancyDeleteMessageHandler));    
    }

    MultiPartitionLocalObjectManager::~MultiPartitionLocalObjectManager ()
    {
    }

    MultiPartitionLocalObjectManager  *MultiPartitionLocalObjectManager::getInstance()
    {
        static MultiPartitionLocalObjectManager *pLocalObjectManager = new MultiPartitionLocalObjectManager ();

        WaveNs::prismAssert (NULL != pLocalObjectManager, __FILE__, __LINE__);

        return (pLocalObjectManager);
    }

    string  MultiPartitionLocalObjectManager::getServiceName()
    {
        return ("Multi Partition Local Service");
    }

    PrismServiceId  MultiPartitionLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage *MultiPartitionLocalObjectManager::createMessageInstance (const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case MULTI_PARTITION_DELETE_LOCAL_PARTITION_INSTANCE :
                 pPrismMessage = new MultiPartitionDeleteLocalPartitionMessage ();
                 prismAssert (NULL != pPrismMessage , __FILE__, __LINE__);
                 break;

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    void  MultiPartitionLocalObjectManager::tenancyDeleteMessageHandler( MultiPartitionDeletePartitionMessage *pMultiPartitionDeletePartitionMessage)
    {
     // Validations and Queries should go here

          PrismSynchronousLinearSequencerStep  sequencerSteps[] =
          {
               reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MultiPartitionLocalObjectManager::validateInputs),
               // reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MultiPartitionLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
               reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MultiPartitionLocalObjectManager::processPartitionObjectCleanup),

               // reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
               reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
               reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MultiPartitionLocalObjectManager::prismSynchronousLinearSequencerFailedStep)

          };

          PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMultiPartitionDeletePartitionMessage, this,
                                                                                                       sequencerSteps, 
                                                                                                       sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

          WaveNs::prismAssert (NULL != pPrismSynchronousLinearSequencerContext, __FILE__, __LINE__);
        
          pPrismSynchronousLinearSequencerContext->execute ();
     }

     ResourceId   MultiPartitionLocalObjectManager::validateInputs (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) 
     {
         ResourceId status = WAVE_MESSAGE_SUCCESS;

         tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionLocalObjectManager::validateInputs called " );

         MultiPartitionDeleteLocalPartitionMessage   *pDeleteMTMessage = reinterpret_cast<MultiPartitionDeleteLocalPartitionMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
         WaveNs::prismAssert (NULL != pDeleteMTMessage, __FILE__, __LINE__);
         
         do {
                // TODO: Add any checks here. 
         } while(0);

         if (true == pDeleteMTMessage->getIsPartialCleanup ())
         {
             ResourceId partialCleanupTag = pDeleteMTMessage->getPartialCleanupTag ();

             trace (TRACE_LEVEL_INFO, string ("MultiPartitionLocalObjectManager::validateInputs : PartialCleanup : partialCleanupTag [") + FrameworkToolKit::localizeToSourceCodeEnum (partialCleanupTag) + string ("]"));
         }
         else
         {
             trace (TRACE_LEVEL_INFO, string ("MultiPartitionLocalObjectManager::validateInputs : Non-partial Cleanup (delete)."));
         }

         return (status);     
     }

     ResourceId   MultiPartitionLocalObjectManager::processPartitionObjectCleanup (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) 
     {
         ResourceId status = WAVE_MESSAGE_SUCCESS;

         tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionLocalObjectManager::processPartitionObjectCleanup  called. " );

         MultiPartitionDeleteLocalPartitionMessage   *pDeleteMTMessage = reinterpret_cast<MultiPartitionDeleteLocalPartitionMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
         WaveNs::prismAssert (NULL != pDeleteMTMessage, __FILE__, __LINE__);

         UI32 senderServiceId   = pDeleteMTMessage->getSenderServiceId ();
         string partitionName      = pDeleteMTMessage->getPartitionName();

         ObjectId ownerPartitionObjectId   = pDeleteMTMessage->getOwnerPartitionManagedObjectId (); 

         MultiPartitionCleanupAgent  *pMultiPartitionCleanupAgent  = new MultiPartitionCleanupAgent(this, partitionName, senderServiceId , ownerPartitionObjectId);
         prismAssert(NULL != pMultiPartitionCleanupAgent, __FILE__ , __LINE__);

         if (true == pDeleteMTMessage->getIsPartialCleanup ())
         {
             ResourceId partialCleanupTag = pDeleteMTMessage->getPartialCleanupTag ();
             pMultiPartitionCleanupAgent->setPartialCleanupTag (partialCleanupTag);
         }

         tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionLocalObjectManager::processPartitionObjectCleanup: Before CleanupAgent is called. " );
         status = pMultiPartitionCleanupAgent->execute();
         tracePrintf(TRACE_LEVEL_INFO,  "MultiPartitionLocalObjectManager::processPartitionObjectCleanup: After  CleanupAgent is called. " );

         delete pMultiPartitionCleanupAgent;

         return (status);     
     }

}

