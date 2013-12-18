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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.    *
 *   All rights reserved.                                            *
 *   Author : jiyer                                                  *
 *********************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "File/Global/FileObjectManager.h"
#include "File/Local/FileAbortFileTransferMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

    FileObjectManager::FileObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
          // Nothing in this class.
          // TraceLevel traceLevel = TRACE_LEVEL_DEVEL; setTraceLevel (traceLevel);
    }

    FileObjectManager::~FileObjectManager ()
    {
    }

    FileObjectManager  *FileObjectManager::getInstance()
    {
        static FileObjectManager *pFileObjectManager = new FileObjectManager ();

        WaveNs::prismAssert (NULL != pFileObjectManager, __FILE__, __LINE__);

        return (pFileObjectManager);
    }

    string  FileObjectManager::getClassName()
    {
        return ("File Service");
    }

PrismServiceId  FileObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FileObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
        switch (operationCode)
        {

            // Currently there is no opcode to be processed at the Global File Service level.
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FileObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
          tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
          return NULL;
    }

  void FileObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
  {
          pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
          pWaveAsynchronousContextForBootPhases->callback ();
          
  }
  
  void FileObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
          // We do not have to do any thing here.  
          pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
          pWaveAsynchronousContextForBootPhases->callback ();
}

  void FileObjectManager::disable (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
  {
          pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
          pWaveAsynchronousContextForShutDownPhases->callback ();
    }

  void FileObjectManager::shutdown (PrismAsynchronousContext *pPrismAsynchronousContext)
  {
          pPrismAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
          pPrismAsynchronousContext->callback ();
          // TODO: Do any further cleanup here.
  }

  void FileObjectManager::listenForEvents	(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
  {
          pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
          pWaveAsynchronousContextForBootPhases->callback ();
  }

void FileObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

//     ResourceId status = WAVE_MESSAGE_SUCCESS;
     vector<LocationId>::iterator itr = failedLocationIds.begin();
     tracePrintf(TRACE_LEVEL_INFO, "[%s]:[%d]  Called ", __FUNCTION__ , __LINE__ );
     
     for (; itr != failedLocationIds.end() ; itr++) 
     {
          tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] Location [%d] is not reachable event ...\n", __FUNCTION__, __LINE__, *itr);
     }

     // Remove the VcsCluster Managed Objects and SNDB Managed Objects
     switch (failoverReason) {
          case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED:
          {
               tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] controlled failover event received. ...\n", __FUNCTION__, __LINE__);
               break;
          }
          case FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED:
          {
               tracePrintf (TRACE_LEVEL_INFO, "[%s]:[%d] un-controlled failover event received. ...\n", __FUNCTION__, __LINE__);
               break;
          }
          default:
               prismAssert (false, __FILE__, __LINE__);
               break;
     }

     //TODO: Local service on the current location seems to be not getting this message delivered. Need to look into it.
#if 0     
     vector<LocationId> connectedLocationsVector;
     FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
     for(itr = connectedLocationsVector.begin(); itr != connectedLocationsVector.end(); itr++)
     {
          FileAbortFileTransferMessage  *pAbortFTMessage = new FileAbortFileTransferMessage(failedLocationIds);
          WaveNs::prismAssert (NULL != pAbortFTMessage, __FILE__, __LINE__);     
          status = sendSynchronously (pAbortFTMessage, *itr);
          if (WAVE_MESSAGE_SUCCESS != status)
          {
               trace (TRACE_LEVEL_DEBUG, string ("FileObjectManager::failover : Sending a message to [") +  *itr + "] to abort any pending file transfer failed , Error code =[" + FrameworkToolKit::localize(status) + "]\n");
          }
          delete pAbortFTMessage;
          pAbortFTMessage = NULL;
     }
     
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
     
#endif

    FileAbortFileTransferMessage  *pAbortFTMessage = new FileAbortFileTransferMessage (failedLocationIds);
    WaveNs::prismAssert (NULL != pAbortFTMessage, __FILE__, __LINE__);
    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&FileObjectManager::TriggerFailOverCallback), pFailoverAsynchronousContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pAbortFTMessage);
       
    trace (TRACE_LEVEL_DEBUG, "PersistenceObjectManager::savePrismConfigurationAtAllLocationsStep : sending to cluster");
    sendToWaveCluster (pWaveSendToClusterContext);
          
}

void FileObjectManager::TriggerFailOverCallback( WaveSendToClusterContext *pWaveSendToClusterContext )
{
    ResourceId   sendToClusterCompletionStatus       = pWaveSendToClusterContext->getCompletionStatus ();
    FailoverAsynchronousContext *pFailoverAsynchronousContext = reinterpret_cast<FailoverAsynchronousContext *> (pWaveSendToClusterContext->getPCallerContext ());

    trace (TRACE_LEVEL_DEBUG, "FileObjectManager::failover : status " + FrameworkToolKit::localize (sendToClusterCompletionStatus));

    prismAssert (NULL != pFailoverAsynchronousContext, __FILE__, __LINE__);

    pFailoverAsynchronousContext->setCompletionStatus(pWaveSendToClusterContext->getCompletionStatus());
    delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
    delete pWaveSendToClusterContext;
    

    pFailoverAsynchronousContext->callback ();
    // pFailoverAsynchronousContext->executeNextStep (sendToClusterCompletionStatus);
}




}
