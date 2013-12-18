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

/**
 *@file VcsFabDeleteClusterWorker.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * 
 * Description: This file has the implementation of the
 *              Worker class which is responsible for delete
 *              Cluster operation.
 * 
 * Author :     Aashish Akhouri
 * Date :       11/16/2010
 *  
 */

#include "Framework/ObjectModel/PrismElement.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Global/VcsAddSwitchMessage.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "VcsFabric/Global/VcsFabMessagingContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "VcsFabric/Global/VcsFabDeleteClusterWorker.h"

namespace DcmNs
{

/** 
 *Name:  VcsFabDeleteClusterWorker 
 *Description: Constructor 
 * 
 * @param pWaveObjectManager: pointer to the ObjectManager
 */
 VcsFabDeleteClusterWorker:: VcsFabDeleteClusterWorker(WaveObjectManager* pWaveObjectManager)
           :WaveWorker (pWaveObjectManager)
{
    addOperationMap (VCS_FABRIC_DELETE_CLUSTER, reinterpret_cast<PrismMessageHandler> (& VcsFabDeleteClusterWorker::deleteClusterMessageHandler));


}

/** 
 * Name : ~ VcsFabDeleteClusterWorker
 * Description : Destructor
 */
 VcsFabDeleteClusterWorker::~ VcsFabDeleteClusterWorker()
{
}

/** 
 *Name: createMessageInstance 
 * Description: Message Creation
 * 
 * @param: operationCode : opcode for delete cluster
 * 
 * @return: PrismMessage* : returns the base class pointer
 *                          pointing to VcsDeleteClusterMessage
 */
PrismMessage * VcsFabDeleteClusterWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_FABRIC_DELETE_CLUSTER :
            pPrismMessage = new VcsDeleteClusterMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


/** 
 *Name:         deleteClusterMessageHandler 
 * 
 *Description: Creates the sequencer and kick starts the
 *             delete Cluster processing.It has the following
 *             steps
 *             1. Delete the VcsClusterManagedObject
 *             2. Send the deleteCluster Message to Cluster
 *             Service
 * 
 *@param1:     VcsDeleteClusterMessage    pointer to 
 *             DeleteClusterMessage
 *                                      
 *@return:     none  
 */
void  VcsFabDeleteClusterWorker::deleteClusterMessageHandler(VcsDeleteClusterMessage * pVcsDeleteClusterMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabDeleteClusterWorker::deleteClusterMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (& VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (& VcsFabDeleteClusterWorker::sendDeleteClusterMessageToClusterStep),
        reinterpret_cast<PrismLinearSequencerStep> (& VcsFabDeleteClusterWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (& VcsFabDeleteClusterWorker::prismLinearSequencerFailedStep)
    };

    
     VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsDeleteClusterMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

     //No hold all is required here
     pVcsFabMessagingContext->start ();   
}

/** 
 * Name: deleteVcsClusterManagedObjectStep 
 * Description: Deletes the VcsClusterManaged object from the 
 *              database
 *  
 * 
 * @param pVcsFabMessagingContext: pointer to VcsFabMessagingContext
 */
void VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
   trace (TRACE_LEVEL_INFO, string("VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep : Entering"));

   VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance();

   VcsClusterManagedObject *pVcsClusterManagedObject = pVcsFabObjectManager->getVcsClusterManagedObject ();

   if (NULL == pVcsClusterManagedObject)
   {
     trace (TRACE_LEVEL_ERROR, string("VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep : VCS cluster not in database"));
   }
   else
   {
       ResourceId status = FRAMEWORK_ERROR;
       startTransaction ();

       delete pVcsClusterManagedObject ;
       pVcsFabObjectManager->clearClusterGUID();

       status = commitTransaction ();

       if (FRAMEWORK_SUCCESS == status)
       {
            trace (TRACE_LEVEL_SUCCESS, " VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep : Successfully deleted VCS cluster from Database.");
       }
       else
       {
            trace (TRACE_LEVEL_ERROR, " VcsFabDeleteClusterWorker::deleteVcsClusterManagedObjectStep : Failed to delete VCS cluster from Database.");
       }
   }

   //Note that even if there is no VcsClusterManagedObject 
   //We will still be going to the next step
   pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

}


/** 
 * Name: sendDeleteClusterMessageToClusterStep
 * Description: This method sends the Delete Cluster message 
 *              asynchronously to the Cluster Service 
 * 
 * @param pVcsFabMessagingContext: pointer to the context
 */
void VcsFabDeleteClusterWorker::sendDeleteClusterMessageToClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{

    trace (TRACE_LEVEL_SUCCESS, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageToClusterStep :Entering");
    ClusterObjectManagerDeleteClusterMessage   *pClusterObjectManagerDeleteClusterMessage = new  ClusterObjectManagerDeleteClusterMessage ();

    ResourceId status = send(pClusterObjectManagerDeleteClusterMessage ,reinterpret_cast<PrismMessageResponseHandler> (&VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack), pVcsFabMessagingContext);


    if (WAVE_MESSAGE_SUCCESS == status)
    {

      trace (TRACE_LEVEL_SUCCESS, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageToClusterStep : Successfully sent DeleteClusterMessage message to cluster");
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageToClusterStep : Could not send ClusterObjectManagerDeleteClusterMessage framework cluster  Status : " + FrameworkToolKit::localize (status));
        
        delete pClusterObjectManagerDeleteClusterMessage;
        pVcsFabMessagingContext->executeNextStep(status);
        
    }    
}


/** 
 * Name: sendDeleteClusterMessageCallBack 
 * Description: Callback function for delete cluster message 
 * sent to the Cluster Service 
 *  
 * @param frameworkStatus: Status returned for framework 
 *                         processing
 * @param pClusterObjectManagerDeleteClusterMessage: pointer to 
 *                                                   the Message
 *                                                   being sent
 *                                                   to the
 *                                                   cluster
 *                                                   service
 * @param pContext: pointer to VcsFabmessaging context that was 
 *                  passed to f/w when send was called
 */
void  VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack(FrameworkStatus frameworkStatus,ClusterObjectManagerDeleteClusterMessage   *pClusterObjectManagerDeleteClusterMessage, void *pContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack: Starting");


    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (pClusterObjectManagerDeleteClusterMessage!=NULL, __FILE__, __LINE__);
        completionStatus = pClusterObjectManagerDeleteClusterMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            trace (TRACE_LEVEL_SUCCESS, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack: Successfully deleted framework cluster");
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack : Failed to delete cluster. status : " + FrameworkToolKit::localize (completionStatus));
        }
    }
    else
    {
       completionStatus = frameworkStatus;
       trace (TRACE_LEVEL_ERROR, "VcsFabDeleteClusterWorker::sendDeleteClusterMessageCallBack : Could not send DeleteClusterMessage. Status : " + FrameworkToolKit::localize (completionStatus));
    }

    delete pClusterObjectManagerDeleteClusterMessage; 

    VcsFabMessagingContext* pVcsFabMessagingContext = static_cast <VcsFabMessagingContext* > (pContext);
    prismAssert (pVcsFabMessagingContext!=NULL,__FILE__,__LINE__);


    pVcsFabMessagingContext->executeNextStep(completionStatus);
    return;
}

}
