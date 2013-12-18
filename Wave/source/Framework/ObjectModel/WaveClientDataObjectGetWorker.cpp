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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.                *
 *   All rights reserved.                                                   *
 *   Author : Anil C                                                        *
 ***************************************************************************/
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Framework/ObjectModel/WaveClientDataObjectGetWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Types/Types.h"
#include "ManagementInterface/ClientInterface/UnifiedClientGetDataFromClientMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace WaveNs
{

WaveClientDataObjectGetWorker::WaveClientDataObjectGetWorker (WaveObjectManager *pWaveObjectManager)
 : WaveWorker (pWaveObjectManager)
{
    addOperationMap (WAVE_OBJECT_MANAGER_GET_CLIENT_DATA, reinterpret_cast<PrismMessageHandler> (&WaveClientDataObjectGetWorker::getDataFromClientHandler));
}

WaveClientDataObjectGetWorker::~WaveClientDataObjectGetWorker ()
{
}

PrismMessage *WaveClientDataObjectGetWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case WAVE_OBJECT_MANAGER_GET_CLIENT_DATA :
            pPrismMessage = new WaveObjectManagerGetDataFromClientMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void WaveClientDataObjectGetWorker::getDataFromClientHandler (WaveObjectManagerGetDataFromClientMessage *pWaveObjectManagerGetDataFromClientMessage)
{
    vector<LocationId> selectedLocationsVector;
    pWaveObjectManagerGetDataFromClientMessage->getSelectedNodes (selectedLocationsVector );

    trace (TRACE_LEVEL_DEVEL, string("WaveClientDataObjectGetWorker::getDataFromClientHandler : Service Name is ") +  m_pWaveObjectManager->getName() );

    if ( (true == pWaveObjectManagerGetDataFromClientMessage->getSendToClusterRequested()) &&
         ( ! ( (1 == selectedLocationsVector.size()) && (FrameworkToolKit::getThisLocationId() == selectedLocationsVector[0])) ) )
    {
        //send to clusterNodes only if it has been requested and 
        //the list does not have this node alone     
        trace (TRACE_LEVEL_DEVEL, "WaveClientDataObjectGetWorker::getDataFromClientHandler : sendToClusterRequested- ");
        getDataFromClientHandlerForSendToCluster(pWaveObjectManagerGetDataFromClientMessage);
    }
    else
    {
        //Get Data from Client directly     
        trace (TRACE_LEVEL_DEVEL, "WaveClientDataObjectGetWorker::getDataFromClientHandler : send To Client - ");
        getDataFromClientHandlerForSendToClient(pWaveObjectManagerGetDataFromClientMessage);
    }

}

void WaveClientDataObjectGetWorker::getDataFromClientHandlerForSendToClient (WaveObjectManagerGetDataFromClientMessage *pWaveObjectManagerGetDataFromClientMessage)
{
    ResourceId status = WAVE_MESSAGE_ERROR;

    UnifiedClientGetDataFromClientMessage *pMessage   = NULL;

    string  clientName ;
    pWaveObjectManagerGetDataFromClientMessage->getClientName ( clientName );

    pMessage = new UnifiedClientGetDataFromClientMessage (pWaveObjectManagerGetDataFromClientMessage->getNumberOfRecordsRequested(), pWaveObjectManagerGetDataFromClientMessage->getCommandCode(), pWaveObjectManagerGetDataFromClientMessage->getCommandType());

    pMessage->setNumberOfCommandStrings (pWaveObjectManagerGetDataFromClientMessage->getNumberOfCommandStrings ());
    pMessage->setCommandStrings (pWaveObjectManagerGetDataFromClientMessage->getCommandStrings ());

    pWaveObjectManagerGetDataFromClientMessage->transferAllBuffers(pMessage);   

    status = sendSynchronouslyToWaveClient (clientName, pMessage);
    
    bool enablePaginate = false;

    if (status == WAVE_MESSAGE_SUCCESS)
    {
        status = pMessage->getCompletionStatus ();

        if ( status != WAVE_MESSAGE_SUCCESS )
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientDataObjectGetWorker::getDataFromClientHandlerForSendToClient : Could not get the Operational Data From client - " + clientName);
        }
        else
        {
            trace (TRACE_LEVEL_DEVEL, "WaveClientDataObjectGetWorker::getDataFromClientHandlerForSendToClient : Successfully got the Operational Data From client - " + clientName);
            enablePaginate = pMessage->getEnablePaginate();
            pWaveObjectManagerGetDataFromClientMessage->copyClientDataFromMessage (pMessage);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientDataObjectGetWorker::getDataFromClientHandlerForSendToClient : Could not send message to get Operational Data From client - " + clientName);
    }

    pWaveObjectManagerGetDataFromClientMessage->addToMoreRecordsVector ( enablePaginate );

    pWaveObjectManagerGetDataFromClientMessage->addStatusPropagation ( status, string(""), FrameworkToolKit::getThisLocationId());

    pWaveObjectManagerGetDataFromClientMessage->setCompletionStatus ( status );
    
    delete pMessage;    

    reply (pWaveObjectManagerGetDataFromClientMessage);
}

void WaveClientDataObjectGetWorker::getDataFromClientHandlerForSendToCluster (WaveObjectManagerGetDataFromClientMessage *pWaveObjectManagerGetDataFromClientMessage)
{
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientDataObjectGetWorker::sendMessageToAllNodesStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientDataObjectGetWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&WaveClientDataObjectGetWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pWaveObjectManagerGetDataFromClientMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->start ();

}

void WaveClientDataObjectGetWorker::sendMessageToAllNodesStep ( PrismLinearSequencerContext *pPrismLinearSequencerContext )
{
    trace (TRACE_LEVEL_DEVEL, "WaveClientDataObjectGetWorker::sendMessageToAllNodesStep : entered-");
    
    WaveObjectManagerGetDataFromClientMessage *pWaveObjectManagerGetDataFromClientMessage = reinterpret_cast<WaveObjectManagerGetDataFromClientMessage *>(pPrismLinearSequencerContext->getPPrismMessage());
    prismAssert ( NULL != pWaveObjectManagerGetDataFromClientMessage, __FILE__, __LINE__);            

    string clientName;
    pWaveObjectManagerGetDataFromClientMessage->getClientName (clientName);
        
    vector<LocationId> selectedLocationsVector;
    pWaveObjectManagerGetDataFromClientMessage->getSelectedNodes (selectedLocationsVector );        

    WaveObjectManagerGetDataFromClientMessage *pMessageToAllNodes = new WaveObjectManagerGetDataFromClientMessage( pWaveObjectManagerGetDataFromClientMessage->getLocalPluginServiceCode(), pWaveObjectManagerGetDataFromClientMessage->getCommandCode(), pWaveObjectManagerGetDataFromClientMessage->getCommandType());

    pMessageToAllNodes->setNumberOfCommandStrings (pWaveObjectManagerGetDataFromClientMessage->getNumberOfCommandStrings ());
    pMessageToAllNodes->setCommandStrings (pWaveObjectManagerGetDataFromClientMessage->getCommandStrings ());

    vector<UI32> bufferTags;
    pWaveObjectManagerGetDataFromClientMessage->getBufferTags(bufferTags);
 
    //Input buffer size is expected to <= 1 as per requirement
    // To support more size we need to change the client input structure
    prismAssert ( 1 >= bufferTags.size(), __FILE__, __LINE__);

    pMessageToAllNodes->setClientName (clientName);
    pMessageToAllNodes->setNumberOfRecordsRequested( pWaveObjectManagerGetDataFromClientMessage->getNumberOfRecordsRequested() );

    //Copy input buffer into the outgoing message
     pMessageToAllNodes->copyAllBuffersFromMessage( pWaveObjectManagerGetDataFromClientMessage );

        //TestingBegin
        /*
        for ( UI32 j = 0 ; j < bufferTags.size() ; j++)
        {
            UI32 size = 0;    
            void * pBuffer = NULL;
            pBuffer = pWaveObjectManagerGetDataFromClientMessage->findBuffer(bufferTags[j],size);
            emp_t *pEmp_t = (emp_t *) pBuffer;
            prismAssert ( NULL != pBuffer, __FILE__, __LINE__);
            //Dont free the buffer since it is owned by message 
            pMessageToAllNodes->addBuffer(bufferTags[j], size, pBuffer, false);
            trace (TRACE_LEVEL_INFO, string("WaveClientDataObjectGetWorker::sendMessageToAllNodesStep :input size is ") + size + (" ,x is -> ") + pEmp_t->x + (" flag is -> ") + pEmp_t->flag);
            pBuffer = NULL;
        }

        */ 
        //Testing End

    WaveSendToClusterContext *pWaveSendToClusterContext = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback), pPrismLinearSequencerContext);
        
    pWaveSendToClusterContext->setPPrismMessageForPhase1(pMessageToAllNodes);

    if ( 0 != selectedLocationsVector.size() )
    {
        pWaveSendToClusterContext->setLocationsToSendToForPhase1( selectedLocationsVector );
    }

    pWaveSendToClusterContext->setPartialSuccessFlag( true );

    sendToWaveCluster (pWaveSendToClusterContext);

}

void WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback ( WaveSendToClusterContext *pWaveSendToClusterContext )
{
    trace (TRACE_LEVEL_INFO, string("WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback : got response from sendToWaveCluster "));

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());

    if (NULL != pPrismLinearSequencerContext )
    {
        WaveObjectManagerGetDataFromClientMessage *pWaveObjectManagerGetDataFromClientMessage = dynamic_cast<WaveObjectManagerGetDataFromClientMessage *>( pPrismLinearSequencerContext->getPPrismMessage ()) ;
 
        if ( NULL != pWaveObjectManagerGetDataFromClientMessage )
        {
            vector<LocationId>  locations           = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
            UI32                numberOfLocations   = locations.size ();

            for (UI32 i = 0; i < numberOfLocations; i++)
            {
                WaveObjectManagerGetDataFromClientMessage *pResponseMessage = dynamic_cast<WaveObjectManagerGetDataFromClientMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));

                if (NULL == pResponseMessage)
                {
                    trace (TRACE_LEVEL_FATAL, string("WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback : dynamic cast from ResultingMessageForPhase1 failed for Location - ") + locations[i] ) ;
                    prismAssert (false, __FILE__, __LINE__);
                }
            
                if (WAVE_MESSAGE_SUCCESS == pResponseMessage->getCompletionStatus())
                {
                    vector<bool> moreRecordsVector ; 
                    pResponseMessage->getMoreRecordsVector ( moreRecordsVector );
                    pWaveObjectManagerGetDataFromClientMessage->addToMoreRecordsVector( moreRecordsVector[0] );
                    pWaveObjectManagerGetDataFromClientMessage->copyAllBuffersFromMessage( pResponseMessage );
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, string("WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback : Location - ") + locations[i] + (", has Completion Status - ") + FrameworkToolKit::localize(pResponseMessage->getCompletionStatus() ) ) ;
                    pWaveObjectManagerGetDataFromClientMessage->addToMoreRecordsVector(false);
                }
            }

            delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
            delete pWaveSendToClusterContext;

            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
        }    
        else
        {
            trace (TRACE_LEVEL_FATAL, "WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback : dynamic cast to WaveObjectManagerGetDataFromClientMessage in PrismLinearSequencerContext failed.");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, " WaveClientDataObjectGetWorker::sendMessageToAllNodesStepCallback : PrismLinearSequencerContext pointer is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

}
