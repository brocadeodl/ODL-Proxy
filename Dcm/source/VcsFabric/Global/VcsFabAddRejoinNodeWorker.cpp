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
 *@file VcsFabAddRejoinNodeWorker.cpp
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 *
 * Description: This file has the implementation of the
 *              Worker class which is responsible for all
 *              cluster related operations like add, rejoin and
 *              delete
 *
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
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Global/VcsClusterManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "VcsFabric/Global/VcsFabAddRejoinNodeWorker.h"
#include "VcsFabric/Global/VcsFabConfigSnapshotWorker.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Types/UI32Range.h"
#include "FabricService/Local/FabricServiceDisableIslsToNeighborMessage.h"
#include <algorithm>
#include "fabric/fabric.h"

extern "C"
{
    #include "raslog/raslog.h"
    #include "raslog/raslog_vcs.h"
}

namespace DcmNs
{

/**
 *Name: VcsFabAddRejoinNodeWorker
 *Description: Constructor
 *
 * @param pWaveObjectManager
 */
VcsFabAddRejoinNodeWorker::VcsFabAddRejoinNodeWorker(WaveObjectManager* pWaveObjectManager)
           :WaveWorker (pWaveObjectManager)
{
    addOperationMap (VCS_FABRIC_ADD_SWITCH, reinterpret_cast<PrismMessageHandler> (&VcsFabAddRejoinNodeWorker::addRejoinSwitchMessageHandler));
}

/**
 *
 *
 */
VcsFabAddRejoinNodeWorker::~VcsFabAddRejoinNodeWorker()
{
}


/**
 *Name:        startClusterReformation
 *Description: Creates the sequencer and kick starts the
 *             recreation of a cluster on a reboot
 *
 *@param1:     VcsAddSwitchMessage    pointer to
 *                                    AddSwitchMessage
 *
 *@return:     none

*/

void VcsFabAddRejoinNodeWorker::startClusterReformation (VcsAddSwitchMessage* pVcsAddSwitchMessage)
{

     trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::startClusterReformation: Cluster Already exists");
     PrismLinearSequencerStep sequencerSteps[] =
     {
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::clusterReformationValidateStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prepareFailoverListStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerSucceededStep),
       reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerFailedStep)
      };

      VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsAddSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

      pVcsFabMessagingContext->setClusterReboot(true);

      //No hold all is required here becuase the AddNodeWorker should be able to
      // process multiple requests simultaneously
      pVcsFabMessagingContext->start ();
}

/**
 *Name:        clusterReformationValidateStep
 *Description: Creates the sequencer and kick starts the
 *
 *
 *@param1:     VcsAddSwitchMessage    pointer to
 *                                    AddSwitchMessage
 *
 *@return:     none

*/
void VcsFabAddRejoinNodeWorker::clusterReformationValidateStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
  trace(TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::clusterReformationValidateStep: Entering..");

  VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
  if (!pVcsFabObjectManager->vcsModeEnabled ())
  {
      trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::clusterReformationValidateStep: VCS Mode is disabled. failed");
      pVcsFabMessagingContext->executeNextStep((VCS_CLUSTER_OPERATION_ALL_LOCATION_FAILED));
      return;
  }

  trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::clusterReformationValidateStep:Validation Successful");


  pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
  return;
}

/**
 *Name:        addRejoinSwitchMessageHandler
 *Description: Creates the sequencer and kick starts the
 *             addrejoin processing.
 *
 *@param1:     VcsAddSwitchMessage    pointer to
 *                                    AddSwitchMessage
 *
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::addRejoinSwitchMessageHandler(VcsAddSwitchMessage* pVcsAddSwitchMessage)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabAddRejoinNodeWorker::addRejoinSwitchMessageHandler : Entering ...");


    //If the state of the node is unconfirmed it is a cluster reformation message
    if ((LOCATION_SECONDARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole () ||LOCATION_PRIMARY_UNCONFIRMED == FrameworkToolKit::getThisLocationRole ()))
    {
      startClusterReformation (pVcsAddSwitchMessage);
      return;
    }

    VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance();

    bool  isClusterCreated = pVcsFabObjectManager->vcsClusterMOExists ();
    //Note that this is an asynchronous sequencer because the calls made to the Cluster Service wil be asynchronous
    if (isClusterCreated)
       {

           trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::addRejoinSwitchMessageHandler: Cluster Already exists");
           PrismLinearSequencerStep sequencerSteps[] =
           {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep),
			reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addRejoinValidateRequestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::replaceRejoinSwitchesToClusterStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerFailedStep)
           };

           VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsAddSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

           //No hold all is required here becuase the AddNodeWorker should be able to
           // process multiple requests simultaneously
          pVcsFabMessagingContext->start ();
    }
    else
    {

           trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::addRejoinSwitchMessageHandler: Cluster Does not exist");
           PrismLinearSequencerStep sequencerSteps[] =
           {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addRejoinValidateRequestStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::createClusterStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabAddRejoinNodeWorker::prismLinearSequencerFailedStep)
           };

           VcsFabMessagingContext *pVcsFabMessagingContext = new VcsFabMessagingContext (reinterpret_cast<PrismMessage *> (pVcsAddSwitchMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

          //No hold all is required here becuase the AddNodeWorker should be able to
          // process multiple requests simultaneously
          pVcsFabMessagingContext->start ();
    }

}

/**
 *Name:        addOrRejoinValidateRequestStep
 *Description: validates addSwitch Request.Currently it will
 *             just check for VcsModeEnabled
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::addRejoinValidateRequestStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
    pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
    return;
}

/**
 *Name:        prepareAddRejoinListsStep
 *Description: prepares list of nodes to add or rejoin and
 *             stores them in the context
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
  trace(TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep: Entering..");

  //get the message from the context
  VcsAddSwitchMessage* pAddSwitchMessage = static_cast<VcsAddSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());

  //Add switch can be auto generated when auto cluster formation is on
  //or it can be generated as a result of an add command from cli

  vector<VcsFabricLocalSwitchInfoObject> switchesInfo = pAddSwitchMessage->getRemoteSwitchInfoObjects ();
  vector<VcsFabricLocalSwitchInfoObject> switchesNotAdded;

  UI32 numSwitchesToBeAddedToAddOrRejoinList     = switchesInfo.size ();
  UI32 numberOfSwitchesNotAddedToAddOrRejoinList = 0;

  VcsFabObjectManager *pVcsFabObjectManager = VcsFabObjectManager::getInstance();

  //Determine if each switch is an add or rejoin
  for(UI32 i = 0; i< numSwitchesToBeAddedToAddOrRejoinList; ++i)
  {
      VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = pVcsFabObjectManager->getSwitchObjectFromDomainId (switchesInfo [i].getSwitchDomainId ());

      //If VcsNodeLocalManagedObject is not in the local DB that means that the node needs to be added
      if(NULL == pVcsNodeLocalManagedObject)
      {

          tracePrintf(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep:VcsNodeLocalManagedObject for domainId %u not found in the DB. Adding to the Add List",switchesInfo [i].getSwitchDomainId ());
          pVcsFabMessagingContext->addSwitchInfoToAddList (switchesInfo [i]);

      }
      else
      {
          ResourceId specificStatusOfVcsNodeLocalMangedObject = pVcsNodeLocalManagedObject->getSpecificStatus ();
          string     statusString                             = FrameworkToolKit::localize (specificStatusOfVcsNodeLocalMangedObject);

          //If VcsNodeLocalManagedObject is in the local DB with specific status disconnected from cluster then
          //it needs to be rejoined to the cluster

          if (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER == specificStatusOfVcsNodeLocalMangedObject)
		  {
              trace (TRACE_LEVEL_INFO, string ("VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep:VcsNodeLocalManagedObject for domainId ") + switchesInfo [i].getSwitchDomainId () + string (" found in the DB with specific status: ") + statusString.c_str() + string (". Adding to the Rejoin List"));

              pVcsFabMessagingContext->addSwitchInfoToRejoinList (switchesInfo[i]);
          }
          else if(VCS_FABRIC_LOCAL_SPECIFIC_STATUS_NODE_REPLACEMENT_IN_PROGRESS == specificStatusOfVcsNodeLocalMangedObject)
          {
                trace (TRACE_LEVEL_INFO, string ("VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep:VcsNodeLocalManagedObject for domainId ") + switchesInfo [i].getSwitchDomainId () + string (" found in the DB with specific status: ") + statusString.c_str() + string (". Adding to the Rejoin List"));

                //Reuse the location Id
                switchesInfo[i].setSwitchLocationId (pVcsNodeLocalManagedObject->getLocationId ());

                pVcsFabMessagingContext->addSwitchInfoToReplaceRejoinList (switchesInfo[i]);
          }
          else if( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED  == specificStatusOfVcsNodeLocalMangedObject )
          {
              // Clean up of the node has not happened properly so delete it here. defect --> 405707
              ResourceId status = WAVE_MESSAGE_ERROR;

              UI32 domainId = pVcsNodeLocalManagedObject->getDomainId();
              LocationId locID = pVcsNodeLocalManagedObject->getLocationId();

              startTransaction ();

              delete pVcsNodeLocalManagedObject;

              pVcsNodeLocalManagedObject = NULL;

              status = commitTransaction ();

              if (FRAMEWORK_SUCCESS != status)
              {
                  trace (TRACE_LEVEL_FATAL, string("VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep : Not Able to Delete uncleaned VcsNodeLocalMO with domainID - ") + domainId + (", locationID ") + locID );
                  prismAssert (false, __FILE__, __LINE__);
              }
              else
              {
                  trace (TRACE_LEVEL_INFO, string("VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep : Deleted uncleaned VcsNodeLocalMO with domainID - ") + domainId + (", locationID ") + locID );
                  pVcsFabMessagingContext->addSwitchInfoToAddList (switchesInfo[i]);
              }
          }
          else
          {
              tracePrintf (TRACE_LEVEL_INFO, false, false, "VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep:VcsNodeLocalManagedObject for domainId %u found in the DB  with specific status: %s . Not Added  to Add Or Rejoin Lists\n",switchesInfo[i].getSwitchDomainId (), statusString.c_str());
              ++numberOfSwitchesNotAddedToAddOrRejoinList;
          }

          if (NULL != pVcsNodeLocalManagedObject)
          {
              delete pVcsNodeLocalManagedObject;
          }
      }

  }

  if(numberOfSwitchesNotAddedToAddOrRejoinList == numSwitchesToBeAddedToAddOrRejoinList)
  {
      trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::prepareAddRejoinListsStep:Both Add and Rejoin Lists are empty");

      if(pVcsFabMessagingContext->getClusterReboot())
      {
         pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
      }
      else
      {
         pVcsFabMessagingContext->executeNextStep (VCS_CLUSTER_OPERATION_ALL_LOCATION_FAILED);
      }

      return;
  }


  trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:prepareAddRejoinListsStep:Lists successfully created");
  pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
  return;
}


/**
 * Name:        prepareFailoverListStep
 * Description: prepares list of nodes for failover
 *
 *
 * @param pVcsFabMessagingContext
 */
void VcsFabAddRejoinNodeWorker::prepareFailoverListStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
    trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:prepareFailoverListStep: Entering..");
    vector<WaveManagedObject* > *pResultVcsNodeLocalMO = querySynchronously(VcsNodeLocalManagedObject::getClassName());
    prismAssert(NULL!=pResultVcsNodeLocalMO, __FILE__, __LINE__);

    //domainIds

    UI32               numVcsNodeLocalManagedObjects = pResultVcsNodeLocalMO->size ();
    vector<LocationId> knownLocationIds;
    UI32               numberOfKnownLocations;

    FrameworkToolKit::getKnownLocations (knownLocationIds);

    numberOfKnownLocations = knownLocationIds.size ();

    map<LocationId, bool> knownLocationsMap;

    for(UI32 i = 0; i < numVcsNodeLocalManagedObjects; ++i)
    {

        VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject* > ((*pResultVcsNodeLocalMO)[i]);

        prismAssert(NULL!=pVcsNodeLocalManagedObject, __FILE__, __LINE__);

        ResourceId specificStatusOfVcsNodeLocalMangedObject = pVcsNodeLocalManagedObject->getSpecificStatus ();
        string statusString = FrameworkToolKit::localize(specificStatusOfVcsNodeLocalMangedObject);

        UI32 nodeDomainId = pVcsNodeLocalManagedObject->getDomainId();
        VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance();
        //If VcsNodeLocalManagedObject is in the local DB with specific status disconnected from cluster then
        //it needs to be rejoined to the cluster

        if(nodeDomainId != pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId())
        {
            LocationId failedLocationId = pVcsNodeLocalManagedObject->getLocationId();

            if ((CCM_ONLY == DcmToolKit::getDistributionMode ()) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_STARTED == specificStatusOfVcsNodeLocalMangedObject) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE2_COMPLETED == specificStatusOfVcsNodeLocalMangedObject) || (VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_ADD_PHASE3_STARTED == specificStatusOfVcsNodeLocalMangedObject))
            {
                pVcsFabMessagingContext->addToControlledFailoverList (failedLocationId );

                knownLocationsMap [failedLocationId] = true;

                tracePrintf (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:prepareFailoverListStep: LocatinId %u added to controlled failover list",failedLocationId );
            }
            else
             {
                 pVcsFabMessagingContext->addToUncontrolledFailoverList (failedLocationId );

                 knownLocationsMap [failedLocationId] = false;

                 tracePrintf (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:prepareFailoverListStep: LocatinId %u added to uncontrolled failover list",failedLocationId );
            }
        }

    }

    for (UI32 j = 0; j < numberOfKnownLocations; j++)
    {
        trace (TRACE_LEVEL_INFO, string ("Trying to find the location ") + knownLocationIds[j]);

        map<LocationId, bool>::iterator element     = knownLocationsMap.find (knownLocationIds[j]);
        map<LocationId, bool>::iterator endElement  = knownLocationsMap.end ();

        if (element == endElement)
        {
           pVcsFabMessagingContext->addToControlledFailoverList (knownLocationIds[j]);
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultVcsNodeLocalMO);
    pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
}

/**
 *
 *
 * @param pVcsFabMessagingContext
 */
void  VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    trace(TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootStep: Entering..");
    FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage* pEstablishPrincipalAfterClusterRebootMessage = new FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage();

   ResourceId status = send(pEstablishPrincipalAfterClusterRebootMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback), pVcsFabMessagingContext);

   if(WAVE_MESSAGE_SUCCESS != status)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:establishPrincipalAfterClusterRebootStep:Failed to send EstablishPrincipalAfterClusterRebootMessage to framework " + FrameworkToolKit::localize (status));
       delete pEstablishPrincipalAfterClusterRebootMessage;
       pVcsFabMessagingContext->executeNextStep(status);
    }
    else
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:establishPrincipalAfterClusterRebootStep:EstablishPrincipalAfterClusterRebootMessage sent successfully to framework ");

    }

    return;

}

/**
 *
 *
 * @param frameworkStatus
 * @param pEstablishPrincipalAfterClusterRebootMessage
 * @param pContext
 */
void VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback (FrameworkStatus frameworkStatus,FrameworkObjectManagerEstablishPrincipalAfterClusterRebootMessage* pEstablishPrincipalAfterClusterRebootMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback : Starting");
    VcsFabMessagingContext* pVcsFabMessagingContext = static_cast <VcsFabMessagingContext* > (pContext);

    prismAssert (pVcsFabMessagingContext!=NULL, __FILE__, __LINE__);

    VcsAddSwitchMessage* pAddSwitchMessage = static_cast <VcsAddSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());

    prismAssert (pEstablishPrincipalAfterClusterRebootMessage!=NULL, __FILE__, __LINE__);
    prismAssert(pAddSwitchMessage!=NULL, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pEstablishPrincipalAfterClusterRebootMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {

            VcsFabObjectManager* pVcsFabObjectManager = VcsFabObjectManager::getInstance ();
            pVcsFabObjectManager->setPrincipalFlag (true);
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback : Successfully established this node as new principal");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback : Failed to establish this node as new Principal::status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = frameworkStatus;
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::establishPrincipalAfterClusterRebootCallback: Could not send EstablishPrincipalAfterClusterRebootMessage to framework   Status : " + FrameworkToolKit::localize (status));

    }

    delete pEstablishPrincipalAfterClusterRebootMessage;
    pVcsFabMessagingContext->executeNextStep(status);


}

/**
 *
 *
 * @param pVcsFabMessagingContext
 */
void  VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    trace(TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep: Entering..");

    FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage ();
    vector<LocationId> failedLocationIds = pVcsFabMessagingContext->getSwitchesForControlledFailover ();

    if(failedLocationIds.size() ==0)
    {
        trace(TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep:No failed LocationIds in the failover list");
        delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;

        pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }


    for(UI32 i=0; i<failedLocationIds.size (); ++i)
    {
        pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->addFailedLocationId(failedLocationIds[i]);
    }

    pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->setIsControlledFailoverInSpecialCase (true);

    ResourceId status = send(pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::sendFailoverMessageAfterClusterRebootCallback), pVcsFabMessagingContext);

    if(WAVE_MESSAGE_SUCCESS != status)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep: Failed to send FrameworkObjectManagerSecondaryNodeFailureNotificationMessage to framework " + FrameworkToolKit::localize (status));
       delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;
       pVcsFabMessagingContext->executeNextStep(status);
    }
    else
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendControlledFailoverMessageAfterClusterRebootStep: FrameworkObjectManagerSecondaryNodeFailureNotificationMessage sent successfully to framework ");
    }

    return;
}

void  VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    trace(TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep: Entering..");

    FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage = new FrameworkObjectManagerSecondaryNodeFailureNotificationMessage ();
    vector<LocationId> failedLocationIds = pVcsFabMessagingContext->getSwitchesForUncontrolledFailover ();

    if(failedLocationIds.size() ==0)
    {
      trace(TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep:No failed LocationIds in the failover list");
      pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
      return;
    }


    for(UI32 i=0; i<failedLocationIds.size (); ++i)
    {
        pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->addFailedLocationId(failedLocationIds[i]);
    }

    ResourceId status = send(pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::sendFailoverMessageAfterClusterRebootCallback), pVcsFabMessagingContext);

    if(WAVE_MESSAGE_SUCCESS != status)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep: Failed to send FrameworkObjectManagerSecondaryNodeFailureNotificationMessage to framework " + FrameworkToolKit::localize (status));
       delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;
       pVcsFabMessagingContext->executeNextStep(status);
    }
    else
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker::sendUncontrolledFailoverMessageAfterClusterRebootStep: FrameworkObjectManagerSecondaryNodeFailureNotificationMessage sent successfully to framework ");
    }

    return;
}

/**
 *
 *
 * @param frameworkStatus
 * @param pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage
 * @param pContext
 */
void VcsFabAddRejoinNodeWorker::sendFailoverMessageAfterClusterRebootCallback (FrameworkStatus frameworkStatus,FrameworkObjectManagerSecondaryNodeFailureNotificationMessage *pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::sendFailoverMessageAfterClusterRebootCallback : Starting");
    VcsFabMessagingContext* pVcsFabMessagingContext = static_cast <VcsFabMessagingContext* > (pContext);

    prismAssert (pVcsFabMessagingContext!=NULL, __FILE__, __LINE__);

    VcsAddSwitchMessage* pAddSwitchMessage = static_cast <VcsAddSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());

    prismAssert (pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage!=NULL, __FILE__, __LINE__);
    prismAssert(pAddSwitchMessage!=NULL, __FILE__, __LINE__);

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {

            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker:: sendFailoverMessageAfterClusterRebootCallback: Successfully failedover");

        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker:: sendFailoverMessageAfterClusterRebootCallback: Failed to failover: Status:"+ FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = frameworkStatus;
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::: sendFailoverMessageAfterClusterRebootCallback:Could not send SecondaryNodeFailureNotificationMessage to framework   Status : " + FrameworkToolKit::localize (status));

    }

    delete pFrameworkObjectManagerSecondaryNodeFailureNotificationMessage;
    pVcsFabMessagingContext->executeNextStep(status);


}

/**
 *Name:         createClusterStep
 *Description:  This function sends an asynchronous cluster
 *              creation request to the ClusterService.
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::createClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{
  trace(TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::createClusterStep: Entering..");

  //get the vector of nodes to be added from the context
  vector<VcsFabricLocalSwitchInfoObject> addSwitchesInfo  = pVcsFabMessagingContext->getSwitchesToBeAdded ();
  UI32                                   numSwitchesToAdd = addSwitchesInfo.size ();

  //Create the vector of ipAddresses and ports
   vector <string> ipAddresses;
   vector <SI32>   ports;

   for (UI32 i = 0; i < numSwitchesToAdd; i++)
   {
       ipAddresses.push_back (addSwitchesInfo[i].getSwitchClusterManagementIpAddress ());
       ports.push_back (addSwitchesInfo[i].getSwitchClusterManagementPort ());

       trace (TRACE_LEVEL_INFO, string ("VcsFabAddRejoinNodeWorker::createClusterStep: iPAddress = ")+ipAddresses[i]+" port = "+ports[i]);
   }

   //send the message asynchronously to the framework
    ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage = new  ClusterObjectManagerCreateClusterMessage (ipAddresses, ports);

    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        pClusterObjectManagerCreateClusterMessage->setIsAddNodeForSpecialCaseFlag (true);
    }

    ResourceId status = send (pClusterObjectManagerCreateClusterMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::createClusterCallBack), pVcsFabMessagingContext);

    if(WAVE_MESSAGE_SUCCESS != status)
    {
       WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsFabAddRejoinNodeWorker:createClusterStep:Failed to send message ClusterObjectManagerCreateClusterMessage sent to framework ") + FrameworkToolKit::localize (status));

       delete pClusterObjectManagerCreateClusterMessage;

       pVcsFabMessagingContext->executeNextStep(status);
    }
    else
    {
       WaveNs::trace (TRACE_LEVEL_INFO, "VcsFabAddRejoinNodeWorker:createClusterStep:ClusterObjectManagerCreateClusterMessage sent successfully to framework ");
    }

    return;

}

/**
 * Name: createClusterCallBack
 * Description: This is the callback from the the Framework to
 *              the asynchronous create cluster message that was
 *              sent.
 *
 * @param frameworkStatus : returns the status of framework
 *                        processing
 * @param pClusterObjectManagerCreateClusterMessage : Clsuter
 *                                                  creation
 *                                                  message to
 *                                                  the cluster
 *                                                  Servide
 *
 * @param pContext: pointer to VcsFabMessagingContext returned
 *                  from the Framework
 */
void  VcsFabAddRejoinNodeWorker::createClusterCallBack (FrameworkStatus frameworkStatus,ClusterObjectManagerCreateClusterMessage *pClusterObjectManagerCreateClusterMessage, void *pContext)
{
    VcsFabMessagingContext* pVcsFabMessagingContext = reinterpret_cast <VcsFabMessagingContext* > (pContext);

    prismAssert (pVcsFabMessagingContext!=NULL, __FILE__, __LINE__);

    VcsAddSwitchMessage* pAddSwitchMessage = dynamic_cast <VcsAddSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());

    prismAssert (pClusterObjectManagerCreateClusterMessage != NULL, __FILE__, __LINE__);
    prismAssert (pAddSwitchMessage != NULL, __FILE__, __LINE__);

    ResourceId                             status           = WAVE_MESSAGE_SUCCESS;
    UI32                                   i                = 0, domainId;
    vector<VcsFabricLocalSwitchInfoObject> addSwitchesInfo  = pVcsFabMessagingContext->getSwitchesToBeAdded ();
    UI32                                   numSwitchesToAdd = addSwitchesInfo.size();

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        status = pClusterObjectManagerCreateClusterMessage->getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker::createClusterCallBack : Successfully created cluster");
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::createClusterCallBack : Failed to create cluster::status : " + FrameworkToolKit::localize (status));
        }
    }
    else
    {
        status = frameworkStatus;
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::createClusterCallBack : Could not send ClusterObjectManagerCreateClusterMessage to framework cluster  Status : " + FrameworkToolKit::localize (status));
    }

    // Variabled used for RASLOGs
    vector<string>  principalPublicIpAddress    = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
    UI32            vcsId                       = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectVcsId ();

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        for (i = 0; i < numSwitchesToAdd; i++)
        {
            domainId = addSwitchesInfo[i].getSwitchDomainId ();

            pAddSwitchMessage->addToFailedDomainIdVector (domainId);

            tracePrintf(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::createClusterCallBack : Status No Success Adding domain id %u to failed list of VcsAddSwitchMessage.", domainId);
        }

        if (( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()))
        {
            raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1002, principalPublicIpAddress[0].c_str (), vcsId, FrameworkToolKit::localize (status).c_str ());
        }
    }
    else
    {
        vector<UI32>      successfulDomainIdVector;
        map<UI32, string> successfulSwitchPublicIPAddressMap;
        string            switchIp;
        UI32              switchPort;

        for (i = 0; i < numSwitchesToAdd; i++)
        {
            switchIp = addSwitchesInfo[i].getSwitchClusterManagementIpAddress ();;
            switchPort = addSwitchesInfo[i].getSwitchClusterManagementPort ();
            domainId   = addSwitchesInfo[i].getSwitchDomainId ();

            if (WAVE_MESSAGE_SUCCESS != pClusterObjectManagerCreateClusterMessage->getNodeStatus (switchIp, switchPort))
            {
                pAddSwitchMessage->addToFailedDomainIdVector (domainId);
                tracePrintf(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::createClusterCallBack : Adding domain id %u to failed list of VcsAddSwitchMessage.", domainId);
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_DEVEL, string("VcsFabAddRejoinNodeWorker::createClusterCallBack : Successfully able to add domainId = ") + domainId);

                successfulDomainIdVector.push_back (domainId);
                successfulSwitchPublicIPAddressMap[domainId] = (addSwitchesInfo[i].getSwitchPublicIpAddress ())[0];
            }
        }


        // Setup member node information for RASLOG:
        // 1) Number of successfully added member nodes.
        // 2) Rbridge Ids (domainId) as a string range
        // 3) Switch public Ip addresses

        UI32 numberOfSuccessfulNodes = successfulDomainIdVector.size ();

        if (0 < numberOfSuccessfulNodes)
        {
            sort (successfulDomainIdVector.begin (), successfulDomainIdVector.end ());

            string rbridgeIdRange = UI32Range::getUI32RangeStringFromVector (successfulDomainIdVector);
            string switchPublicIpList = "";

            WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabAddRejoinNodeWorker::createClusterCallBack : Rbridge Id Range = ") + rbridgeIdRange);
            for (i = 0; i < numberOfSuccessfulNodes; i++)
            {
                switchPublicIpList += string (successfulSwitchPublicIPAddressMap[successfulDomainIdVector[i]]);
                WaveNs::trace (TRACE_LEVEL_INFO, string("VcsFabAddRejoinNodeWorker::createClusterCallBack : Successful Public IP List = ") + switchPublicIpList);

                if (i < (numberOfSuccessfulNodes - 1))
                {
                    switchPublicIpList += ", ";
                }
            }


            vector<Attribute *> logArguments;

            logArguments.push_back (new AttributeUI32 (numberOfSuccessfulNodes, "NumberOfSuccessfulNodes"));
            logArguments.push_back (new AttributeString (rbridgeIdRange, "RbridgeIdRange"));
            logArguments.push_back (new AttributeString (switchPublicIpList, "SwitchPublicIpList"));

            string createClusterStatus = FrameworkToolKit::localize (VCS_CLUSTER_RASLOG_STATUS_CREATE, logArguments);

            for (i = 0; i < static_cast<UI32> (logArguments.size ()); i++)
            {
                delete logArguments[i];
            }

            logArguments.clear ();

            if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
            {
                raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1001, principalPublicIpAddress[0].c_str (), vcsId, createClusterStatus.c_str ());
            }
        }
    }

    delete pClusterObjectManagerCreateClusterMessage;

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        status = VCS_CLUSTER_OPERATION_ALL_LOCATION_FAILED;
    }

    pVcsFabMessagingContext->executeNextStep(status);
}

/**
 *Name:         addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep
 *Description:  This function determines which nodes were added
 *              and adds their OIDs to the VCSClusterManaged
 *              Object
 *
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep (VcsFabMessagingContext* pVcsFabMessagingContext)
{

    //Get the list of nodes from the DB
   vector<WaveManagedObject *> *pResultVcsNodeLocalMO = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

   if (NULL == pResultVcsNodeLocalMO)
   {
         trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep:results pointer from querySynchronously is NULL");
         prismAssert (false, __FILE__, __LINE__);
   }

   UI32 numVcsNodeLocalManagedObjects = pResultVcsNodeLocalMO->size();

   if (0 == numVcsNodeLocalManagedObjects)
   {
         trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep:No managed object in the vector");
         prismAssert (false, __FILE__, __LINE__);
   }


    //Loop through the list of nodes to be added and check if they are in the MO list, if so
    //then their OID can be added to VcsCluster Managed Object
    vector<VcsFabricLocalSwitchInfoObject> addSwitchesInfo      = pVcsFabMessagingContext->getSwitchesToBeAdded();
    UI32                                   numSwitchesToBeAdded = addSwitchesInfo.size();
    UI32                                   numNodesAdded        = 0;

    VcsFabObjectManager* pVcsFabObjectManager         = VcsFabObjectManager::getInstance ();
    VcsClusterManagedObject *pVcsClusterManagedObject = pVcsFabObjectManager->getVcsClusterManagedObject ();


    VcsFabricLocalObjectManager* pVcsFabricLocalObjectManager = VcsFabricLocalObjectManager::getInstance ();

    UI32     myVcsId    = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectVcsId();
    UI32     myDomainId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectDomainId();
    ObjectId myObjectId = pVcsFabricLocalObjectManager->getThisVcsNodeLocalManagedObjectObjectId();

    startTransaction ();
    if(NULL == pVcsClusterManagedObject)
    {
       Uuid uniqueUUID;
       pVcsFabObjectManager ->setClusterGUID(uniqueUUID);
       trace (TRACE_LEVEL_DEBUG, string("VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep:set guid to VcsFabObjectManager: ") + pVcsFabObjectManager->getClusterGUID ().toString());

       pVcsClusterManagedObject = new VcsClusterManagedObject (pVcsFabObjectManager);
       prismAssert (NULL != pVcsClusterManagedObject, __FILE__, __LINE__);

       pVcsClusterManagedObject->setVcsClusterId(myVcsId);
       pVcsClusterManagedObject->setPrincipalSwitchObjectId (myObjectId);
       pVcsClusterManagedObject->setPrincipalSwitchDomainId (myDomainId);
       pVcsClusterManagedObject->setClusterGUID(pVcsFabObjectManager->getClusterGUID ());
       trace (TRACE_LEVEL_DEBUG, string("VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep:set guid to VcsClusterManagedObject: ") + pVcsClusterManagedObject->getClusterGUID ().toString());

       pVcsClusterManagedObject->addSwitchObjectId (myObjectId);
    }
    else
    {
       updateWaveManagedObject (pVcsClusterManagedObject);
    }

    vector<ObjectId>                    clusterSwitchList       = pVcsClusterManagedObject->getSwitchObjectIds ();
    vector<ObjectId>::iterator          it;
    bool                                duplicateEntry          = false;
    VcsAddSwitchMessage*                pAddSwitchMessage       = static_cast <VcsAddSwitchMessage* >(pVcsFabMessagingContext->getPPrismMessage());
    vector<UI32>                        failedDomainIdVector    = pAddSwitchMessage->getFailedDomainIdVector ();

    for(UI32 i = 0; i < numSwitchesToBeAdded ; ++i)
    {
       for(UI32 j = 0; j < numVcsNodeLocalManagedObjects ; ++j )
       {
          VcsNodeLocalManagedObject* pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *>((*pResultVcsNodeLocalMO)[j]);

          prismAssert(NULL!=pVcsNodeLocalManagedObject, __FILE__, __LINE__);

          UI32      nodeDomainId = pVcsNodeLocalManagedObject->getDomainId ();
          ObjectId  nodeObjectId = pVcsNodeLocalManagedObject->getObjectId();

          //If domainIds match then add the oid of the switch to the VcsCluster Managed Object

          if(nodeDomainId == addSwitchesInfo[i].getSwitchDomainId ())
          {
            //Failed Domains should not be added
            if (isDomaidIdInFailedList (nodeDomainId, failedDomainIdVector))
            {
                break;
            }

            string statusString = FrameworkToolKit::localize( pVcsNodeLocalManagedObject->getSpecificStatus ());

            tracePrintf (TRACE_LEVEL_INFO, false, false, "VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep:VcsNodeLocalManagedObject for domainId %u found in the DB with specific status: %s. Adding to the VcsClusterMO", nodeDomainId, statusString.c_str());

            ++numNodesAdded;

            for (it = clusterSwitchList.begin (); it != clusterSwitchList.end (); it++)
            {
                if ((*it) == nodeObjectId)
                {
                    trace (TRACE_LEVEL_FATAL, "VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep: Adding duplicate entry to VcsClusterManagedObject");
                    duplicateEntry = true;
                    break;
                }
            }

            if (duplicateEntry)
            {
                duplicateEntry = false;
                break;
            }

             //Add the node OID to the VcsClusterManaged object
             pVcsClusterManagedObject->addSwitchObjectId(nodeObjectId);
             break;
          }
       }

    }

    //Update the cluster status
    vector<LocationId> connectedLocationsVector;
    FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
    UI32 numberOfConnectedNodes = connectedLocationsVector.size ();

    if((numVcsNodeLocalManagedObjects-1) == numberOfConnectedNodes)
    {
       pVcsClusterManagedObject->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_GOOD);
       pVcsClusterManagedObject->setSpecificStatus(VCS_CLUSTER_SPECIFIC_STATUS_ALL_NODES_IN_CLUSTER);
    }
    else
    {
       pVcsClusterManagedObject->setGenericStatus(WAVE_MANAGED_OBJECT_GENERIC_STATUS_DEGRADED);
       pVcsClusterManagedObject->setSpecificStatus(VCS_CLUSTER_SPECIFIC_STATUS_NODES_DISCONNECTED_FROM_CLUSTER);
    }

    ResourceId status = commitTransaction ();

    if(FRAMEWORK_SUCCESS != status)
    {
      trace(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addOidsOfVcsNodeLocalManagedObjectToVcsClusterManagedObjectStep: Failed to commit transaction" + FrameworkToolKit::localize (status));
        prismAssert (false, __FILE__, __LINE__);
    }

    delete pVcsClusterManagedObject;

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultVcsNodeLocalMO);

    pVcsFabMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

/**
 *Name:         rejoinSwitchesToClusterStep
 *Description:  This function figures out the nodes to add and
 *              sends an asynchronous request to the
 *              ClusterService
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{

    vector<VcsFabricLocalSwitchInfoObject> rejoinSwitchesInfo  = pVcsFabMessagingContext->getSwitchesToBeRejoined();
    UI32                                   numSwitchesToRejoin = rejoinSwitchesInfo.size();

    if(0 == numSwitchesToRejoin )
    {
        tracePrintf(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterStep: No switches to rejoin");
        pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
        return;
    }

    ClusterObjectManagerRejoinNodeMessage *pRejoinMessage = new ClusterObjectManagerRejoinNodeMessage ();

    for (UI32 i= 0; i < numSwitchesToRejoin; i++)
    {
        pRejoinMessage->setNodeIpAddress  (rejoinSwitchesInfo[i].getSwitchClusterManagementIpAddress ());
        pRejoinMessage->setNodePort       (rejoinSwitchesInfo[i].getSwitchClusterManagementPort ());
        pRejoinMessage->setNodeLocationId (rejoinSwitchesInfo[i].getSwitchLocationId ());
    }

    //Asynchronous send and now waiting for a callback
    ResourceId status = send (pRejoinMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack), pVcsFabMessagingContext);

    if (WAVE_MESSAGE_SUCCESS == status)
    {

    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterStep : Could not send rejoin message to ClusterObjectManage to add switch to framework cluster  Status : " + FrameworkToolKit::localize (status));
        //Note that here we are declaring success , even though rejoin send failed becuase we still want to attempt the
        //add node

        delete pRejoinMessage;
        pVcsFabMessagingContext->setRejoinNodeStatus (status);
        pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

    }

}

/**
 *Name:         replaceRejoinSwitchesToClusterStep
 *Description:  This function figures out the nodes to replacer rejoin and
 *              sends an asynchronous request to the
 *              ClusterService
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::replaceRejoinSwitchesToClusterStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    vector<VcsFabricLocalSwitchInfoObject> rejoinSwitchesInfo         = pVcsFabMessagingContext->getSwitchesToBeReplaceRejoined ();
    UI32                                   numSwitchesToReplaceRejoin = rejoinSwitchesInfo.size();

    if(0 == numSwitchesToReplaceRejoin )
    {
        tracePrintf(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::replaceRejoinSwitchesToClusterStep: No switches to rejoin");
        pVcsFabMessagingContext->executeNextStep((WAVE_MESSAGE_SUCCESS));
        return;
    }

    ClusterObjectManagerRejoinNodeMessage *pRejoinMessage = new ClusterObjectManagerRejoinNodeMessage ();

    pRejoinMessage->setIsReplaceRejoin (true);

    for (UI32 i= 0; i < numSwitchesToReplaceRejoin; i++)
    {
        pRejoinMessage->setNodeIpAddress  (rejoinSwitchesInfo[i].getSwitchClusterManagementIpAddress ());
        pRejoinMessage->setNodePort (rejoinSwitchesInfo[i].getSwitchClusterManagementPort ());
        pRejoinMessage->setNodeLocationId (rejoinSwitchesInfo[i].getSwitchLocationId ());
    }

    //Asynchronous send and now waiting for a callback
    ResourceId status = send (pRejoinMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack), pVcsFabMessagingContext);

    if (WAVE_MESSAGE_SUCCESS == status)
    {

      trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker::replaceRejoinSwitchesToClusterStep : Successfully sent replace rejoin message to cluster");
      //Note that here we are not calling execute next step because we are waiting for a callback and the next step should be executed
      //from the callback
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::replaceRejoinSwitchesToClusterStep : Could not send rejoin message to ClusterObjectManage to add switch to framework cluster  Status : " + FrameworkToolKit::localize (status));
        //Note that here we are declaring success , even though rejoin send failed becuase we still want to attempt the
        //add node

        delete pRejoinMessage;
        pVcsFabMessagingContext->setRejoinNodeStatus (status);
        pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

    }

}

/**
 * Name: rejoinSwitchesToClusterCallBack
 * Description: Callback from framework to the asynchronous
 *              rejoin sent to the Cluster Service. Even if a
 *              failure is returned in the callback we still
 *              proceeed to process the add.
 *
 * @param frameworkStatus : Status of framework processing
 * @param pClusterObjectManagerRejoinNodeMessage : ptr to
 *                                               message
 * @param pContext: pointer to VcsFabMessagingContext
 */
void  VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack(FrameworkStatus frameworkStatus,ClusterObjectManagerRejoinNodeMessage   *pClusterObjectManagerRejoinNodeMessage, void *pContext)
{
    trace (TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Starting");
    VcsFabMessagingContext* pVcsFabMessagingContext = static_cast <VcsFabMessagingContext* > (pContext);

    prismAssert (pVcsFabMessagingContext!=NULL,__FILE__,__LINE__);

    VcsAddSwitchMessage* pAddSwitchMessage = static_cast <VcsAddSwitchMessage* > (pVcsFabMessagingContext->getPPrismMessage());

    prismAssert (pClusterObjectManagerRejoinNodeMessage!=NULL, __FILE__, __LINE__);
    prismAssert(pAddSwitchMessage!=NULL, __FILE__, __LINE__);

    ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
    UI32 i = 0;
    UI32 domainId;
    vector<VcsFabricLocalSwitchInfoObject> rejoinSwitchesInfo = pVcsFabMessagingContext->getSwitchesToBeRejoined();
    UI32 numSwitchesToRejoin = rejoinSwitchesInfo.size();

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        completionStatus = pClusterObjectManagerRejoinNodeMessage->getCompletionStatus ();
        //ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Successfully rejoined switches to framework cluster");

        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Failed to rejoin switches to framework cluster status : " + FrameworkToolKit::localize (completionStatus));

        }
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallback : Could not send rejoin message to ClusterObjectManagerCreateClusterMessage handler to add switch to framework cluster  Status : " + FrameworkToolKit::localize (completionStatus));

        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        for (i = 0; i < numSwitchesToRejoin; i++)
        {
            domainId = rejoinSwitchesInfo[i].getSwitchDomainId ();

            pAddSwitchMessage->addToFailedDomainIdVector (domainId);

            trace (TRACE_LEVEL_ERROR, string ("VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Adding domain id ") + domainId + string (" to failed list of VcsAddSwitchMessage.") + string (" failure reason is ") + FrameworkToolKit::localize (pClusterObjectManagerRejoinNodeMessage->getNodeStatus (rejoinSwitchesInfo[i].getSwitchClusterManagementIpAddress (), rejoinSwitchesInfo[i].getSwitchClusterManagementPort ())));

            if (WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH == pClusterObjectManagerRejoinNodeMessage->getNodeStatus (rejoinSwitchesInfo[i].getSwitchClusterManagementIpAddress (), rejoinSwitchesInfo[i].getSwitchClusterManagementPort ()))
            {
                // segment this Switch
                wwn_t emptyWWNC;

                memset ((char *)&emptyWWNC, 0, sizeof (wwn_t));

                WorldWideName emptyWWN  (string(wwnfmt(&emptyWWNC)));
                WorldWideName switchWWN = DcmToolKit::getSwitchWwnFromDomainId (domainId);

                if (emptyWWN == switchWWN)
                {
                    trace (TRACE_LEVEL_ERROR, string ("VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack VcsNodeLocalManagedObject should be present in DB for Rejoin)"));
                    prismAssert (false, __FILE__, __LINE__);
                }

                vector<LocationId> connectedLocationIds;

                FrameworkToolKit::getFullyConnectedLocations (connectedLocationIds);
                connectedLocationIds.push_back(FrameworkToolKit::getThisLocationId());

                UI32 numberOfConnectedLocations = connectedLocationIds.size ();

                for(UI32 location = 0; location < numberOfConnectedLocations; location++)
                {
                    LocationId locationId = connectedLocationIds [location];

                    FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage = new FabricServiceDisableIslsToNeighborMessage (switchWWN, WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH);

                    sendOneWay (pFabricServiceDisableIslsToNeighborMessage, locationId);
                }
            }
        }
    }
    else
    {
        vector<string>  principalPublicIpAddress    = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
        UI32            vcsId                       = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectVcsId ();
        string switchIp;
        UI32 switchPort;
        vector<string>  switchPublicIpAddress;

        for (i = 0; i < numSwitchesToRejoin; i++)
        {
            switchIp              = rejoinSwitchesInfo[i].getSwitchClusterManagementIpAddress ();
            switchPort            = rejoinSwitchesInfo[i].getSwitchClusterManagementPort ();
            domainId              = rejoinSwitchesInfo[i].getSwitchDomainId ();
            switchPublicIpAddress = rejoinSwitchesInfo[i].getSwitchPublicIpAddress ();

            ResourceId nodeStatus = pClusterObjectManagerRejoinNodeMessage->getNodeStatus (switchIp, switchPort);

            if (WAVE_MESSAGE_SUCCESS != nodeStatus)
            {
                pAddSwitchMessage->addToFailedDomainIdVector (domainId);

                if (WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH == nodeStatus)
                {
                    //segment switch

                    wwn_t emptyWWNC;

                    memset ((char *)&emptyWWNC, 0, sizeof (wwn_t));

                    WorldWideName emptyWWN  (string(wwnfmt(&emptyWWNC)));
                    WorldWideName switchWWN = DcmToolKit::getSwitchWwnFromDomainId (domainId);

                    if (emptyWWN == switchWWN)
                    {
                        trace (TRACE_LEVEL_ERROR, string ("VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack VcsNodeLocalManagedObject should be present in DB for Rejoin)"));
                        prismAssert (false, __FILE__, __LINE__);
                    }

                    vector<LocationId> connectedLocationIds;

                    FrameworkToolKit::getFullyConnectedLocations (connectedLocationIds);
                    connectedLocationIds.push_back(FrameworkToolKit::getThisLocationId());

                    UI32 numberOfConnectedLocations = connectedLocationIds.size ();

                    for(UI32 location = 0; location < numberOfConnectedLocations; location++)
                    {
                        LocationId locationId = connectedLocationIds [location];

                        FabricServiceDisableIslsToNeighborMessage *pFabricServiceDisableIslsToNeighborMessage = new FabricServiceDisableIslsToNeighborMessage (switchWWN, WAVE_MESSAGE_ERROR_CONFIGURATION_MISMATCH);

                        sendOneWay (pFabricServiceDisableIslsToNeighborMessage, locationId);
                    }
                }

                tracePrintf(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Adding domain id %u to failed list of VcsAddSwitchMessage.", domainId);

                if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                {
                    raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1006, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str (), FrameworkToolKit::localize (nodeStatus).c_str ());
                }
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_DEVEL, string("VcsFabAddRejoinNodeWorker::rejoinSwitchesToClusterCallBack : Successfully able to rejoin domainId = ") + domainId);

                if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                {
                    raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1005, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str ());
                }
            }
        }
    }

    delete pClusterObjectManagerRejoinNodeMessage;

    pVcsFabMessagingContext->setRejoinNodeStatus(completionStatus);
    //Irrespective of whether rejoin succeeded or failed we will go to the next step
    pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

/**
 *Name:         addSwitchesToClusterStep
 *Description:  This function figures out the nodes to add and
 *              sends an asynchronous request to the
 *              ClusterService
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
    vector<VcsFabricLocalSwitchInfoObject> addSwitchesInfo  = pVcsFabMessagingContext->getSwitchesToBeAdded();
    UI32                                   numSwitchesToAdd = addSwitchesInfo.size();

    if(0 == numSwitchesToAdd )
    {
        tracePrintf(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep: No switches to add");
        pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        return;
    }

    ClusterObjectManagerAddNodeMessage *pAddMessage = new ClusterObjectManagerAddNodeMessage ();
    string snapshotFilename ("");
    vector<string> filenames;

    for (UI32 i= 0; i < numSwitchesToAdd; i++)
    {
        snapshotFilename = ("");
        pAddMessage->setSecondaryNodeNameAndPort(addSwitchesInfo[i].getSwitchClusterManagementIpAddress (), addSwitchesInfo[i].getSwitchClusterManagementPort ());
        VcsFabConfigSnapshotWorker::getRestoreTriggeredConfigSnapshotFilenameForDomainId( addSwitchesInfo[i].getSwitchDomainId(), snapshotFilename);

        //snapshotfilename to be syncd is added in to another vector at same index.
        //Filename empty means no file to be syncd
        filenames.push_back(snapshotFilename);

        if ( false == snapshotFilename.empty() )
        {
            trace (TRACE_LEVEL_INFO, string ("VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep: Restore set for domainId = ") + addSwitchesInfo[i].getSwitchDomainId() + (", filename = ") + snapshotFilename );
        }
    }
    pAddMessage->setFilenamesToSync ( filenames );

    if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        pAddMessage->setIsAddNodeForSpecialCaseFlag (true);
    }

    pAddMessage->setNSecondaryNodes (numSwitchesToAdd);
    //Asynchronous send and now waiting for a callback
    ResourceId status = send (pAddMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack), pVcsFabMessagingContext);

    if (WAVE_MESSAGE_SUCCESS == status)
    {
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterStep : Could not send ClusterObjectManagerAddNodeMessage to add switch to framework cluster  Status : " + FrameworkToolKit::localize (status));

        delete pAddMessage;
        pVcsFabMessagingContext->setAddNodeStatus (status);

        //In the next step we will determine whether the entire operation was success or failure.
        pVcsFabMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

    }

}


//TODO: Aashish: If the add is not autogenerated then set I am principal to true

/**
 *Name:         addSwitchesToClusterCallBack
 *Description:  This function determines whether the add request
 *              sent to the cluster failed or succeeded
 *
 *
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */

void VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack (FrameworkStatus frameworkStatus,ClusterObjectManagerAddNodeMessage   *pClusterObjectManagerAddNodeMessage, void *pContext)
{

    trace (TRACE_LEVEL_DEVEL, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Starting");
    VcsFabMessagingContext* pVcsFabMessagingContext = static_cast <VcsFabMessagingContext* > (pContext);

    prismAssert (NULL!=pVcsFabMessagingContext, __FILE__, __LINE__);

    VcsAddSwitchMessage* pAddSwitchMessage = static_cast <VcsAddSwitchMessage* >(pVcsFabMessagingContext->getPPrismMessage());

    prismAssert (NULL!=pClusterObjectManagerAddNodeMessage , __FILE__, __LINE__);
    prismAssert (NULL!=pAddSwitchMessage, __FILE__, __LINE__);

    ResourceId                              completionStatus = WAVE_MESSAGE_SUCCESS;
    UI32                                    i                = 0, domainId;
    vector<VcsFabricLocalSwitchInfoObject>  addSwitchesInfo  = pVcsFabMessagingContext->getSwitchesToBeAdded();
    UI32                                    numSwitchesToAdd = addSwitchesInfo.size();

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        completionStatus = pClusterObjectManagerAddNodeMessage->getCompletionStatus ();
        //ResourceId completionStatus = WAVE_MESSAGE_SUCCESS;
        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Successfully added switch to framework cluster");

        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Failed to add switch to framework cluster: status : " + FrameworkToolKit::localize (completionStatus));

        }
    }
    else
    {
        completionStatus = frameworkStatus;
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Could not send add message to ClusterObjectManagerCreateClusterMessag:  Status : " + FrameworkToolKit::localize (completionStatus));
    }

    if (WAVE_MESSAGE_SUCCESS != completionStatus)
    {
        for (i = 0; i < numSwitchesToAdd; i++)
        {
            domainId = addSwitchesInfo[i].getSwitchDomainId ();

            pAddSwitchMessage->addToFailedDomainIdVector (domainId);
            tracePrintf(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Adding domain id %u to failed list of VcsAddSwitchMessage.", domainId);
        }
    }
    else
    {
        vector<string>  principalPublicIpAddress    = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectPublicIpAddress ();
        UI32            vcsId                       = VcsFabricLocalObjectManager::getInstance ()->getThisVcsNodeLocalManagedObjectVcsId ();
        string switchIp;
        UI32 switchPort;
        vector<string>  switchPublicIpAddress;

        for (i = 0; i < numSwitchesToAdd; i++)
        {
            switchIp              = addSwitchesInfo[i].getSwitchClusterManagementIpAddress ();
            switchPort            = addSwitchesInfo[i].getSwitchClusterManagementPort ();
            domainId              = addSwitchesInfo[i].getSwitchDomainId ();
            switchPublicIpAddress = addSwitchesInfo[i].getSwitchPublicIpAddress ();                

            ResourceId nodeStatus = pClusterObjectManagerAddNodeMessage->getNodeStatus (switchIp, switchPort);

            if (WAVE_MESSAGE_SUCCESS != nodeStatus)
            {
                pAddSwitchMessage->addToFailedDomainIdVector (domainId);
                tracePrintf(TRACE_LEVEL_ERROR, "VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Adding domain id %u to failed list of VcsAddSwitchMessage.", domainId);

                if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                {
                    raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1004, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str (), FrameworkToolKit::localize (nodeStatus).c_str ());
                }
            }
            else
            {
                WaveNs::trace (TRACE_LEVEL_DEVEL, string("VcsFabAddRejoinNodeWorker::addSwitchesToClusterCallBack : Successfully able to add domainId = ") + domainId);

                if ( ( VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == DcmToolKit::getThisVcsNodeClusterType ()) && ( DCM_CCM == DcmToolKit::getDistributionMode ()) )
                {
                    raslog_ext (__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, RASLOG_NOFLAGS, VCS_1003, principalPublicIpAddress[0].c_str (), vcsId, domainId, switchPublicIpAddress[0].c_str ());
                }
            }
        }
    }

    delete pClusterObjectManagerAddNodeMessage;

    //Irrespective of whether add succeeded or failed we will go to the next step
    pVcsFabMessagingContext->setAddNodeStatus(completionStatus);
    pVcsFabMessagingContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}


/**
 *Name:         determineAddRejoinProcessingSuccessStep
 *Description: This function determines whether the request as a
 *             whole should be considered successful. Atleast
 *              one nnode should have been successfully added or
 *              rejoined for the operation to be successful
 *
 *
 *@param1:     VcsFabMessagingContext    pointer to Asynchronous
 *                                       Context
 *@return:     none
 */
void VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep(VcsFabMessagingContext* pVcsFabMessagingContext)
{
     trace(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep: Entering");

     if((WAVE_MESSAGE_SUCCESS == pVcsFabMessagingContext->getAddNodeStatus()) || (WAVE_MESSAGE_SUCCESS == pVcsFabMessagingContext->getRejoinNodeStatus()))
     {
          trace(TRACE_LEVEL_INFO,"VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep: AddOrRejoin Request successfully processed");

          //Update Cluster Status
          pVcsFabMessagingContext->executeNextStep( WAVE_MESSAGE_SUCCESS);
     }
     else
     {
          trace(TRACE_LEVEL_ERROR,"VcsFabAddRejoinNodeWorker::determineAddRejoinProcessingSuccessStep: AddOrRejoin Request Processing failed");
          pVcsFabMessagingContext->executeNextStep (VCS_CLUSTER_OPERATION_ALL_LOCATION_FAILED);

     }

}

bool VcsFabAddRejoinNodeWorker::isDomaidIdInFailedList (UI32 nodeDomainId, vector<UI32> &failedDomainIdVector)
{
    vector<UI32>::iterator it;

    for (it = failedDomainIdVector.begin (); it != failedDomainIdVector.end (); it++)
    {
        if ((*it) == nodeDomainId)
        {
            return true;
        }
    }

    return false;
}

}
