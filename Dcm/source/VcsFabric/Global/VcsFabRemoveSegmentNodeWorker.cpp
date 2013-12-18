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
 *@file VcsFabRemoveSegmentNodeWorker.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file has the implementation of the
 *              Worker class which is responsible for delete
 *              and segment node operation.
 * Author :     Aashish Akhouri
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
#include "VcsFabric/Global/VcsFabRemoveSegmentNodeWorker.h"
#include "fabric/fabric.h"

extern "C"
{
    #include "raslog/raslog.h"
    #include "raslog/raslog_vcs.h"
}

namespace DcmNs
{

/** 
 *Name: VcsFabRemoveSegmentNodeWorker 
 *Description: Constructor 
 * 
 * @param pWaveObjectManager: pointer to WaveObjectManager
 */
VcsFabRemoveSegmentNodeWorker::VcsFabRemoveSegmentNodeWorker(WaveObjectManager* pWaveObjectManager)
           :WaveWorker (pWaveObjectManager)
{
    addOperationMap (VCS_FABRIC_REMOVE_SWITCH, reinterpret_cast<PrismMessageHandler> (&VcsFabRemoveSegmentNodeWorker::removeSwitchMessageHandler));
    addOperationMap (VCS_FABRIC_SEGMENT_SWITCH, reinterpret_cast<PrismMessageHandler> (&VcsFabRemoveSegmentNodeWorker::segmentSwitchMessageHandler));

}

/** 
 * Name: ~VcsFabRemoveSegmentNodeWorker
 * Description: Destructor
 */
VcsFabRemoveSegmentNodeWorker::~VcsFabRemoveSegmentNodeWorker()
{
}

/** 
 * Name: createMessageInstance
 * Description: Create Remove Node Message
 *  
 * @param operationCode
 * 
 * @return PrismMessage* : Message Pointer
 */
PrismMessage *VcsFabRemoveSegmentNodeWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
   
        case VCS_FABRIC_REMOVE_SWITCH :
            pPrismMessage = new VcsRemoveSwitchMessage   ();
            break;
        case VCS_FABRIC_SEGMENT_SWITCH :
            pPrismMessage = new VcsSegmentSwitchMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


/** 
 *Name:         removeSwitchMessageHandler 
 *Description: Creates the sequencer and kick starts the
 *             removeNode processing. 
 * 
 *@param1:     VcsRemoveSwitchMessage    pointer to 
 *                                       RemoveSwitchMessage
 *                                      
 *@return:     none  
 */
void VcsFabRemoveSegmentNodeWorker::removeSwitchMessageHandler(VcsRemoveSwitchMessage * pVcsRemoveSwitchMessage)
{
    trace (TRACE_LEVEL_INFO, "VcsFabRemoveSegmentNodeWorker::removeSwitchMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::sendDeleteNodeToClusterStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::prismLinearSequencerFailedStep)
    };

    
     VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsRemoveSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

     //No hold all is required here becuase the AddNodeWorker should be able to 
     // process multiple requests simultaneously
     pVcsFabMessagingContext->start ();   
}

/** 
 *Name:         sendDeleteNodeToClusterStep 
 *Description: This function prepares the list of nodes to be
 *             sent in the remove message to the Cluster service
 *             and sends the message asynchronously to the
 *             cluster service
 * 
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none  
 */

void VcsFabRemoveSegmentNodeWorker::sendDeleteNodeToClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
  trace(TRACE_LEVEL_INFO, "VcsFabRemoveSegmentNodeWorker::sendDeleteNodeToClusterStep: Entering..");

  //get the message from the context
  VcsRemoveSwitchMessage* pRemoveSwitchMessage = static_cast<VcsRemoveSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());
  vector<WaveManagedObject*> pRemoveNodes ;

  //domainIds
  vector<UI32> switchDomainIds = pRemoveSwitchMessage->getMappedId();
  VcsFabObjectManager* pVcsFabObjectManager = VcsFabObjectManager::getInstance();

//  pVcsFabObjectManager->removeInvalidDomainIdsFromDomainIdList (switchDomainIds);


  UI32 numSwitchesToRemove = switchDomainIds.size ();

  if(0 == numSwitchesToRemove) 
  {
      trace(TRACE_LEVEL_ERROR,"VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: No switches to be removed.");
      pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_ERROR);
      return;
  }

  //Instantiate the remove switch message 
  ClusterObjectManagerDeleteNodeMessage *pClusterObjectManagerDeleteNodeMessage = new  ClusterObjectManagerDeleteNodeMessage ();


  //Populate the removeSwitch message by iterating through the DB
  UI32 numNodesInTheDeleteMessageToCluster = 0;
  for(UI32 i = 0; i< numSwitchesToRemove; ++i) 
  {
      VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = pVcsFabObjectManager->getSwitchObjectFromDomainId(switchDomainIds[i]);

    
      if(NULL == pVcsNodeLocalManagedObject) 
      {
          tracePrintf(TRACE_LEVEL_INFO,"VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep:VcsNodeLocalManagedObject for domainId %u not found in the DB",switchDomainIds[i]);
      }
      else 
      {
        ++numNodesInTheDeleteMessageToCluster;
        pRemoveNodes.push_back( (WaveManagedObject*)pVcsNodeLocalManagedObject);
        
        UI32   switchPort = pVcsNodeLocalManagedObject->getSwitchPort ();

        if (false == DcmToolKit::isIpotEnabled ())
        {
            vector<string> publicIpAddresses = pVcsNodeLocalManagedObject->getPublicIpAddress ();
           
            trace (TRACE_LEVEL_INFO, string ("VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: Ipot is disabled, pubIpAddr = ")+publicIpAddresses[0]+" switchPort = "+switchPort);
            pClusterObjectManagerDeleteNodeMessage->setSecondaryNodeNameAndPort (publicIpAddresses[0], switchPort);

        }
        else
        {
            string ipAddress = DcmToolKit::getIpAddressFromDomainId (switchDomainIds[i]);
            trace (TRACE_LEVEL_INFO, string ("VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: Ipot is enabled, switchIpAddr = ")+ipAddress+" switchPort = "+switchPort);
            pClusterObjectManagerDeleteNodeMessage->setSecondaryNodeNameAndPort (ipAddress, switchPort);
        }

      }
  }
 
    if ( 0 == numNodesInTheDeleteMessageToCluster )
    {
        trace (TRACE_LEVEL_INFO, string ("VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: No nodes to remove"));
        pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return ;
    }
        
 
  pClusterObjectManagerDeleteNodeMessage->setNNodesToDelete(numNodesInTheDeleteMessageToCluster);

  // Set flag to indicate deletion of a disconnected node.  
  if (true ==  pRemoveSwitchMessage->getIsDisconnected()) 
  {
    pClusterObjectManagerDeleteNodeMessage->setIsDisconnected(true);
  }

  WaveMessageStatus status = sendOneWay (pClusterObjectManagerDeleteNodeMessage); //message object gets deleted by sendOneWay */

  if(status != WAVE_MESSAGE_SUCCESS) 
  {
    trace (TRACE_LEVEL_INFO, string ("VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: Failed to send delete node message to the Cluster"));
  }
  else
  {
    trace (TRACE_LEVEL_INFO, "VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep:Delete node sent successfully to the Cluster");
  }
    
    // send RASLOGs for deleted switches
    // Commenting out the below section of code as it was deemed unncessary

    //vector<string>  principalPublicIpAddress    = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
    //UI32            vcsId                       = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectVcsId ();

    //for(UI32 i = 0; i< numNodesInTheDeleteMessageToCluster; i++)
    //{
    //    VcsNodeLocalManagedObject  *pVcsNodeLocalManagedObject  = dynamic_cast <VcsNodeLocalManagedObject*> (pRemoveNodes[i]);

    //    if ( pVcsNodeLocalManagedObject == NULL )
    //    {
    //        tracePrintf (TRACE_LEVEL_ERROR, "VcsFabRemoveSegmentNodeWorker:: sendDeleteNodeToClusterStep: pRemoveNodes cannot have null pointer at position = %d and numNodesInTheDeleteMessageToCluster = %d , PrismAssert here", i, numNodesInTheDeleteMessageToCluster);
    //        prismAssert (false, __FILE__, __LINE__);
    //    }

    //    UI32                        domainId                    = pVcsNodeLocalManagedObject->getDomainId();
    //    vector<string>              switchPublicIpAddress       = pVcsNodeLocalManagedObject->getPublicIpAddress ();

    //    if (WAVE_MESSAGE_SUCCESS != status)
    //    { 
    //        if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
    //        {
    //            raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1008, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str (), FrameworkToolKit::localize (status).c_str ());
    //        }
    //    }
    //    else
    //    {
    //        if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
    //        {
    //            raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1007, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str ());
    //        }
    //    }
    //    delete pVcsNodeLocalManagedObject;
    //}

  pVcsFabMessagingContext->executeNextStep(status);     
  return;
}

/**
 *Name:        segmentSwitchMessageHandler
 *Description: Creates the sequencer and kick starts the
 *             segment Node processing.
 *
 *@param1:     VcsSegmentSwitchMessage   pointer to
 *                                       SegmentSwitchMessage
 *
 *@return:     none
 */
void VcsFabRemoveSegmentNodeWorker::segmentSwitchMessageHandler(VcsSegmentSwitchMessage * pVcsSegmentSwitchMessage)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabRemoveSegmentNodeWorker::segmentSwitchMessageHandler : Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsFabRemoveSegmentNodeWorker::prismLinearSequencerFailedStep)
    };

     VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsSegmentSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
     pVcsFabMessagingContext->start ();
}

/**
 *Name:        sendVcsNodeSegmentSwitchMessageToAllNodesStep
 *Description: This function sends the VcsNodeSegmentSwitchMessage
 *             async to connected nodes in cluster, skips send to the
 *             nodes in the to be segmented list.
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    trace(TRACE_LEVEL_DEVEL, "VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep: Entering..");

    VcsSegmentSwitchMessage* pSegmentSwitchMessage = static_cast<VcsSegmentSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());
    prismAssert (NULL != pSegmentSwitchMessage, __FILE__, __LINE__);

    vector<UI32> segmentReason = pSegmentSwitchMessage->getSegmentReason ();
    vector<UI32> switchDomainId = pSegmentSwitchMessage->getDomainId ();
    UI32 numDomainId = switchDomainId.size();
    vector <WorldWideName> switchWwnList;
    WorldWideName switchWWN;
    wwn_t wwn;

    //Construct zero wwn.    
    wwn_t emptyWWNC;
    memset ((char *)&emptyWWNC, 0, sizeof (wwn_t));
    WorldWideName emptyWWN(string(wwnfmt(&emptyWWNC)));
   
    UI32 i = 0;
    UI32 retCode;
    bool isWwnObtained = true;
    // Get the switch wwn list corresponding to the incoming domain id list.
    for (i = 0; i < numDomainId; i++)    
    {
        isWwnObtained = true;
        switchWWN = DcmToolKit::getSwitchWwnFromDomainId (switchDomainId[i]);

        // This can be a request to segment a node that was never added to the
        // cluster, so MO will be NULL, and zero wwn is returned.
        // So query for the switch WWN from the fabric. If this also fails, proceed 
        // to segmenting remaining domain ids in list, return failure though
        // for the entire operation.
        if (emptyWWN == switchWWN)
        {
            trace (TRACE_LEVEL_DEVEL, string("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : MO IS NULL for domainId : ") + switchDomainId[i]);

            // Invoke fabric API.
            retCode = fabGetWwn(switchDomainId[i], &wwn);
            if (0 != retCode)
            {
                trace (TRACE_LEVEL_ERROR, string ("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : Error getting wwn from fabric"));
                isWwnObtained = false;
            }
            else
            {
                trace (TRACE_LEVEL_DEVEL, string ("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : Got the wwn from the fabric"));
                switchWWN = (UI8 *) wwn.chars;
                // Can be possible during switch disable, fabric building, invalid/unreachable domain cases.
                if (emptyWWN == switchWWN)
                {
                    trace (TRACE_LEVEL_ERROR, string ("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : Got zero wwn from the fabric"));
                    isWwnObtained = false;
                }
            }
        }
       
        if (isWwnObtained)
        {     
            switchWwnList.push_back (switchWWN);
        }
        else 
        {
            pVcsFabMessagingContext->incrementNumberOfFailures ();
            // Remove corresponding reason from list.
            segmentReason.erase (segmentReason.begin() + i); 
        }
    }

    bool skipSendToNode = false;
    vector<LocationId> connectedLocationIds;
    LocationId nodeLocationId;
    UI32 nodeDomainId;
    VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage = NULL;

    // Send VcsNodeSegmentSwitchMessage to connected nodes and to self.
    FrameworkToolKit::getConnectedLocations (connectedLocationIds);
    connectedLocationIds.push_back(FrameworkToolKit::getThisLocationId());
    UI32 numberOfConnectedLocations = connectedLocationIds.size ();

    ++(*pVcsFabMessagingContext);
    
    for(i = 0; i < numberOfConnectedLocations; i++)
    {
        skipSendToNode = false;
        nodeLocationId = connectedLocationIds[i];
        nodeDomainId = DcmToolKit::getDomainIdFromLocationId (nodeLocationId);

        // Do not send the message to any of the switch that need to be segmented.
        for (UI32 j = 0; j < numDomainId; j++)
        {            
            if (nodeDomainId == switchDomainId[j])
            {
                trace (TRACE_LEVEL_INFO, string("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : Skipping segment switch message to domainId ") + switchDomainId[j]);
                skipSendToNode = true;
                break;
            }
        }

        if (skipSendToNode)
        {
            continue;
        }

        pVcsNodeSegmentSwitchMessage = new VcsNodeSegmentSwitchMessage (switchWwnList, segmentReason);

        trace (TRACE_LEVEL_INFO, string("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep : Sending segment switch message to domainId ") + nodeDomainId + string(" to location ") + nodeLocationId);

        WaveMessageStatus status = send (pVcsNodeSegmentSwitchMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback), pVcsFabMessagingContext, 0, nodeLocationId);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabRemoveSegmentNodeWorker::sendVcsNodeSegmentSwitchMessageToAllNodesStep: Could not send segment switch message to location : ") + nodeLocationId + string (" Error Code : ") + FrameworkToolKit::localize (status));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
            delete pVcsNodeSegmentSwitchMessage;
        }
        else
        {
             ++(*pVcsFabMessagingContext);
        }
    }

    --(*pVcsFabMessagingContext);

    pVcsFabMessagingContext->executeNextStep (((pVcsFabMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

/**
 *Name:        sendSegmentSwitchMessageToAllNodesCallback
 *Description: This function is callback for the VcsNodeSegmentSwitchMessage
 *             sent to each node.
 *
 *@param:      frameworkStatus           Framework status of send
 *@param:      pPrismMessage             Original message 
 *@param:      pVcsFabMessagingContext   Pointer to Async Context
 *@return:     none
 */
void VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback (FrameworkStatus frameworkStatus, VcsNodeSegmentSwitchMessage *pVcsNodeSegmentSwitchMessage, VcsFabMessagingContext *pVcsFabMessagingContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback : Starting ...");
    LocationId locationId;

    --(*pVcsFabMessagingContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert ((NULL != pVcsNodeSegmentSwitchMessage), __FILE__, __LINE__);
        ResourceId completionStatus = pVcsNodeSegmentSwitchMessage->getCompletionStatus ();
        locationId = pVcsNodeSegmentSwitchMessage->getReceiverLocationId ();

        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback: Segment switch message failed for location : ") + locationId + string (" Completion status : ") + FrameworkToolKit::localize (completionStatus));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
        }
        else 
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback: Segment switch message suceeded for location : ") + locationId);
        }
    }
    else
    {
        if (NULL != pVcsNodeSegmentSwitchMessage)
        {
            locationId = pVcsNodeSegmentSwitchMessage->getReceiverLocationId ();
            trace (TRACE_LEVEL_ERROR, string ("VcsFabRemoveSegmentNodeWorker::sendSegmentSwitchMessageToAllNodesCallback: Could not send segment switch message to location : ") + locationId + string (" Framework status : ") + FrameworkToolKit::localize (frameworkStatus));
            pVcsFabMessagingContext->incrementNumberOfFailures ();
        }
    }

    if (NULL != pVcsNodeSegmentSwitchMessage)
    {
        delete pVcsNodeSegmentSwitchMessage;
    }

    pVcsFabMessagingContext->executeNextStep (((pVcsFabMessagingContext->getNumberOfFailures ()) > 0) ? WAVE_MESSAGE_ERROR : WAVE_MESSAGE_SUCCESS);
}

}
