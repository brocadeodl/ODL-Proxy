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

/**************************************************************************
*   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Sudhakar Jha                                               *
***************************************************************************/

#include "VcsFabric/Global/VcsClusterConnectionQueryWorker.h"
#include "VcsFabric/Global/VcsClusterConnectionToIpEnqSequencerContext.h"
#include "VcsFabric/Local/VcsIsIpReachableFromNodeMessage.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmResourceIdEnums.h"

namespace DcmNs
{


VcsClusterConnectionQueryWorker::VcsClusterConnectionQueryWorker (VcsFabObjectManager* pVcsFabObjectManager)
    : WaveWorker (pVcsFabObjectManager)    
{
    addOperationMap (VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER, reinterpret_cast<PrismMessageHandler> (& VcsClusterConnectionQueryWorker::executeVcsIsIpReachableFromClusterMessageHandler));
}

VcsClusterConnectionQueryWorker::~VcsClusterConnectionQueryWorker ()
{

}

PrismMessage * VcsClusterConnectionQueryWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER :
            pPrismMessage = new VcsIsIpReachableFromClusterMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}


void VcsClusterConnectionQueryWorker::executeVcsIsIpReachableFromClusterMessageHandler (VcsIsIpReachableFromClusterMessage* vcsIsIpReachableFromClusterMessage)
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsClusterConnectionQueryWorker::validateIpConnectionEnqRequestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsClusterConnectionQueryWorker::validateIpConnectionEnqResponseStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsClusterConnectionQueryWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsClusterConnectionQueryWorker::prismLinearSequencerFailedStep)
    };

    VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext = new VcsClusterConnectionToIpEnqSequencerContext (vcsIsIpReachableFromClusterMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    if (NULL == pVcsClusterConnectionToIpEnqSequencerContext)
    {
        trace (TRACE_LEVEL_FATAL, string ("VcsFabVcsModeWorker::install : VcsClusterConnectionToIpEnqSequencerContext is NULL."));

        prismAssert (false, __FILE__, __LINE__);
    }

    pVcsClusterConnectionToIpEnqSequencerContext->start ();

}


void VcsClusterConnectionQueryWorker::validateIpConnectionEnqRequestStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext)
{
    /* 
     * Validate ip address format if required.  It should allow both "fully qualified system name" & IP-Address.
     * So not doing format validation, will revisit if needed.
    */

    pVcsClusterConnectionToIpEnqSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);

}

void VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext)
{
    VcsIsIpReachableFromClusterMessage* vcsIsIpReachableFromClusterMessage = dynamic_cast<VcsIsIpReachableFromClusterMessage *>(pVcsClusterConnectionToIpEnqSequencerContext->getPPrismMessage());
    string ipaddress = vcsIsIpReachableFromClusterMessage->getIpAddress();


    vector<WaveManagedObject *> *pResultsClusterInfo = querySynchronously (VcsNodeLocalManagedObject::getClassName ());
    prismAssert (NULL != pResultsClusterInfo, __FILE__, __LINE__);

    UI32 nodesInCluster = pResultsClusterInfo->size ();

    ++(*pVcsClusterConnectionToIpEnqSequencerContext);

    for (unsigned int i = 0; i < nodesInCluster; i++)
    {
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResultsClusterInfo)[i]);

        if (NULL == pVcsNodeLocalManagedObject)
        {
            trace (TRACE_LEVEL_ERROR, "VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep: VcsNodeLocalManagedObject is found to be NULL in the vector obtained from query");
            WaveNs::prismAssert (false, __FILE__, __LINE__);
        }

        if ( VCS_FABRIC_LOCAL_SPECIFIC_STATUS_SECONDARY_DISCONNECTED_FROM_CLUSTER == (pVcsNodeLocalManagedObject->getSpecificStatus()))
        {
            // This node is disconnected from cluster.
            continue;
        }

        LocationId locationId = pVcsNodeLocalManagedObject->getLocationId();
        UI32 rbridgeId = pVcsNodeLocalManagedObject->getDomainId ();

        pVcsClusterConnectionToIpEnqSequencerContext->addLocationToMap(locationId, rbridgeId);

        trace (TRACE_LEVEL_DEBUG, string("VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep:: rbridgeId =  ") + rbridgeId);


        VcsIsIpReachableFromNodeMessage* pMessage = new VcsIsIpReachableFromNodeMessage(ipaddress);
        pMessage->setRbridgeId(rbridgeId);


        ResourceId status = send(pMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStepCallback), pVcsClusterConnectionToIpEnqSequencerContext);
        trace (TRACE_LEVEL_DEBUG, "VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep:: sent VcsIsIpReachableFromNodeMessage  to framework");

        if (WAVE_MESSAGE_SUCCESS == status)
        {
            ++(*pVcsClusterConnectionToIpEnqSequencerContext);
            trace (TRACE_LEVEL_DEBUG, "VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep: Successfully sent message to cluster");
       }
        else
        {
           trace (TRACE_LEVEL_ERROR, "VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStep : Could not send Message to framework cluster  Status : " + FrameworkToolKit::localize (status));
           delete pMessage; //TO DO:- Verify whether delete is required here.
         }

    }

    --(*pVcsClusterConnectionToIpEnqSequencerContext);

    if (0 == (pVcsClusterConnectionToIpEnqSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        /* We need to advance to next steps here as all sends may be failed or completed within the same stack */
        pVcsClusterConnectionToIpEnqSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }


}


void VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStepCallback(FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, void *pContext)
{

    VcsClusterConnectionToIpEnqSequencerContext* pVcsClusterConnectionToIpEnqSequencerContext = reinterpret_cast<VcsClusterConnectionToIpEnqSequencerContext *> (pContext);

    --(*pVcsClusterConnectionToIpEnqSequencerContext);

    if (FRAMEWORK_SUCCESS == frameworkStatus)
    {
        prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
        ResourceId completionStatus = pPrismMessage->getCompletionStatus ();
        LocationId r_locationId                  = pPrismMessage->getReceiverLocationId ();
        string ipAddress                   = FrameworkToolKit::getIpAddressForLocationId (r_locationId);
            
        if (WAVE_MESSAGE_SUCCESS != completionStatus)
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStepCallback: Case 1 -  ") + ipAddress + " failed : " + completionStatus);
        }
        else
        {
            VcsIsIpReachableFromNodeMessage* vcsIsIpReachableFromNodeMessage = reinterpret_cast<VcsIsIpReachableFromNodeMessage *>(pPrismMessage);

            IpAddressPingResponseParameters pingResponse;
            pingResponse.isConnected = vcsIsIpReachableFromNodeMessage->getIsReachableFlag();
            pingResponse.rbridgeId = vcsIsIpReachableFromNodeMessage->getRbridgeId();

            pVcsClusterConnectionToIpEnqSequencerContext->addPingResponseToMap(pingResponse.rbridgeId, pingResponse);
        }
    }
    else
    {
        if (NULL != pPrismMessage)
        {
            LocationId locationId = pPrismMessage->getReceiverLocationId ();
            string ipAddress  = FrameworkToolKit::getIpAddressForLocationId (locationId);
            trace (TRACE_LEVEL_ERROR, string ("VcsClusterConnectionQueryWorker::sendIpConnectionEnqRequestStepCallback: Case 2 - Informing ") + ipAddress + " failed : " + frameworkStatus);
        }
    }

    if (NULL != pPrismMessage)
    {
        delete pPrismMessage;
        pPrismMessage = NULL;
    }

    if (0 == (pVcsClusterConnectionToIpEnqSequencerContext->getNumberOfCallbacksBeforeAdvancingToNextStep ()))
    {
        pVcsClusterConnectionToIpEnqSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
    }

}

void VcsClusterConnectionQueryWorker::validateIpConnectionEnqResponseStep(VcsClusterConnectionToIpEnqSequencerContext *pVcsClusterConnectionToIpEnqSequencerContext)
{

    VcsIsIpReachableFromClusterMessage* vcsIsIpReachableFromClusterMessage = reinterpret_cast<VcsIsIpReachableFromClusterMessage*>(pVcsClusterConnectionToIpEnqSequencerContext->getPPrismMessage());

    map<UI32, IpAddressPingResponseParameters>&   allPingResponse =  pVcsClusterConnectionToIpEnqSequencerContext->getPingResponseForAllRbridgeId ();
    map<UI32, IpAddressPingResponseParameters>::iterator element         = allPingResponse.begin ();
    map<UI32, IpAddressPingResponseParameters>::iterator limitingElement = allPingResponse.end ();

    while (element != limitingElement)
    {
        IpAddressPingResponseParameters pingResponse = element->second;

        if(true == pingResponse.isConnected)
        {
            vcsIsIpReachableFromClusterMessage->addRbridgeIdOfConnectedNodes(pingResponse.rbridgeId);
        }
        element++;
    }

        pVcsClusterConnectionToIpEnqSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
