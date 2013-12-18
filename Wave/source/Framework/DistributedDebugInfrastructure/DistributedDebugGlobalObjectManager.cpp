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
 *@file DistributedDebugGlobalObjectManager.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file has all the implementation  for the
 *              all the methods , but mainly it implemenst the
 *              handler for the message received for running
 *              script on all the remote nodes.
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include "Framework/Types/Types.h"
#include "Framework/Types/UI32Range.h"

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureMessages.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugGlobalObjectManager.h" 
#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureTypes.h" 
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Trace/TraceObjectManager.h"


namespace WaveNs
{


/** 
 * Name: DistributedDebugGlobalObjectManager
 * Description: Constructor
 */
DistributedDebugGlobalObjectManager::DistributedDebugGlobalObjectManager ()
    : WaveObjectManager (getClassName ())
{

    addOperationMap (RUN_DEBUG_SCRIPT_MESSAGE, reinterpret_cast<PrismMessageHandler> (&DistributedDebugGlobalObjectManager::runDebugScriptMessageHandler));
}

/** 
 * Name: ~DistributedDebugGlobalObjectManager 
 * Description: Destructor 
 * 
 */
DistributedDebugGlobalObjectManager::~DistributedDebugGlobalObjectManager ()
{
}


/** 
 * Name: getClassName 
 * Description: Returns class name
 * 
 * 
 * @return string
 */
string DistributedDebugGlobalObjectManager::getClassName ()
{
      return ("DistributedDebugGlobalObjectManager");
}


/** 
 * Name: getInstance
 * Description: Singleton
 * 
 * @return DistributedDebugGlobalObjectManager*
 */
DistributedDebugGlobalObjectManager *DistributedDebugGlobalObjectManager::getInstance ()
{
    static DistributedDebugGlobalObjectManager *pDistributedDebugGlobalObjectManager = new DistributedDebugGlobalObjectManager ();

    WaveNs::prismAssert (NULL != pDistributedDebugGlobalObjectManager, __FILE__, __LINE__);

    return (pDistributedDebugGlobalObjectManager);
}

/** 
 * 
 * 
 * 
 * @return PrismServiceId
 */
PrismServiceId DistributedDebugGlobalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

/** 
 * 
 * 
 * 
 * @return string
 */
string DistributedDebugGlobalObjectManager::getPrismServiceName()
{
   return ("Distributed Debug Global Service");
}

/** 
 * 
 * 
 * @param operationCode
 * 
 * @return PrismMessage*
 */
PrismMessage *DistributedDebugGlobalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
    case RUN_DEBUG_SCRIPT_MESSAGE:

	    trace (TRACE_LEVEL_INFO, string ("DistributedDebugGlobalObjectManager::createMessageInstance : operation code : ") + operationCode);
            pPrismMessage = new RunDebugScriptMessage(); 
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DistributedDebugGlobalObjectManager::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}


#if 0
void    DistributedDebugGlobalObjectManager::createBufferForScriptFileStep ()
{

    //Read the input file 
    ifstream  scriptFile;


    string runDebugScriptFileName = pRunDebugScriptMessage->getFullyQualifiedScriptFileName ();
    scriptFile.open (runDebugScriptFileName.c_str (), ios::binary);
    scriptFile.seekg (0, ios::end);
    UI32 sizeOfScriptFile = scriptFile.tellg ();

    prismAssert (0 != sizeOfScriptFile, __FILE__, __LINE__);

    if (0 < sizeOfScriptFile)
    {
        char* pBufferForScriptFile = new char[sizeOfScriptFile];

        prismAssert (NULL != pBufferForScriptFile, __FILE__, __LINE__);

        scriptFile.seekg (0, ios::beg);
        scriptFile.read (pBufferForScriptFile, sizeOfScriptFile);
    }

    scriptFile.close ();

}
#endif

/** 
 * Name: sendDebugScriptToAllNodesStep 
 *  
 * Description: Extracts the message from the messag received 
 * from the client and sends a message to run the script on all 
 * nodes 
 * 
 * @param pRunDebugScriptMessageHandlerContext
 */
void DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesStep (PrismLinearSequencerContext* pRunDebugScriptMessageHandlerContext)
{
    RunDebugScriptMessage *pRunDebugScriptMessage = dynamic_cast<RunDebugScriptMessage *> (pRunDebugScriptMessageHandlerContext->getPPrismMessage());

    prismAssert(NULL!=pRunDebugScriptMessage, __FILE__, __LINE__);

    //Send the Message to Cluster    
    trace (TRACE_LEVEL_DEBUG, "DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesStep :Starting Send To Cluster.");

    RunDebugScriptOnClusterMemberMessage*  pRunDebugScriptOnClusterMemberMessage = new  RunDebugScriptOnClusterMemberMessage(pRunDebugScriptMessage->isScriptNos());
    pRunDebugScriptOnClusterMemberMessage->setScriptString(pRunDebugScriptMessage->getScriptString ());

    WaveSendToClusterContext     *pWaveSendToClusterContext      = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback),pRunDebugScriptMessageHandlerContext);
    
    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pRunDebugScriptOnClusterMemberMessage);
    pWaveSendToClusterContext->setPartialSuccessFlag(true);

    vector<LocationId> locationsToRunDebugScript = pRunDebugScriptMessage->getLocationsToRunDebugScript ();

    if (0 < locationsToRunDebugScript.size ())
    {
        // We will only send to the specified locations

        pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationsToRunDebugScript);

        string locationIdRange = UI32Range::getUI32RangeStringFromVector (locationsToRunDebugScript);

        trace (TRACE_LEVEL_DEBUG, string ("DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesStep : Sending to the following locations: ") + locationIdRange);
    }
    else
    {
        // We default sending to all locations 

        trace (TRACE_LEVEL_DEBUG, "DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesStep : Sending to ALL locations.");
    }

    sendToWaveCluster (pWaveSendToClusterContext);
}

/** 
 * Name: sendDebugScriptToAllNodesCallback
 * Description: Callback for sendToWaveCluster used to send the 
 *              messages to all local services on al nodes 
 * @param pWaveSendToClusterContext
 */
void DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
  trace(TRACE_LEVEL_DEBUG, string("DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback:Entering"));
  //Take out all the messages from the Context 
  vector<LocationId>    locationsToWhichMessageWasSent = pWaveSendToClusterContext->getLocationsToSendToForPhase1  ();
  vector<string>        outputStringVector;

  for (UI32 i = 0; i < locationsToWhichMessageWasSent.size (); ++i) 
  {
      LocationId locationIdOfNode = locationsToWhichMessageWasSent[i];

      tracePrintf(TRACE_LEVEL_INFO, true, false, "DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback: locationId: %d",locationIdOfNode );

      RunDebugScriptOnClusterMemberMessage* pRunDebugScriptOnClusterMemberMessage = static_cast<RunDebugScriptOnClusterMemberMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1 (locationIdOfNode));
      prismAssert(NULL!=pRunDebugScriptOnClusterMemberMessage,__FILE__,__LINE__);

      string tempString = pRunDebugScriptOnClusterMemberMessage->getOutputString ();   
      outputStringVector.push_back (tempString);

      tracePrintf(TRACE_LEVEL_DEBUG, true, false, "DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback: ouputString %s from LocationId: %d", tempString.c_str(),locationIdOfNode);
      tracePrintf(TRACE_LEVEL_DEBUG, true, false, "DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesCallback: locationId: End of loop" );
  }


   PrismLinearSequencerContext*   pRunDebugScriptMessageHandlerContext = static_cast<PrismLinearSequencerContext* > (pWaveSendToClusterContext->getPCallerContext());

   RunDebugScriptMessage* pRunDebugScriptMessage = static_cast<RunDebugScriptMessage *> (pRunDebugScriptMessageHandlerContext->getPPrismMessage());
   pRunDebugScriptMessage->setOutputStringVector(outputStringVector);

   pRunDebugScriptMessageHandlerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

/** 
 * Name: runDebugScriptMessageHandler 
 * Description: Debug script message received from the client 
 *              Handled using a sequencer. 
 * 
 * @param pRunDebugScriptMessage
 */
void  DistributedDebugGlobalObjectManager::runDebugScriptMessageHandler  (RunDebugScriptMessage* pRunDebugScriptMessage)
{

    trace(TRACE_LEVEL_DEBUG,"DistributedDebugGlobalObjectManager::runDebugScriptMessageHandler:Entering:" );

   PrismLinearSequencerStep sequencerSteps[] =
    {
	reinterpret_cast<PrismLinearSequencerStep> (&DistributedDebugGlobalObjectManager::sendDebugScriptToAllNodesStep), 
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedDebugGlobalObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&DistributedDebugGlobalObjectManager::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pRunDebugScriptMessageHandlerContext = new PrismLinearSequencerContext (pRunDebugScriptMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRunDebugScriptMessageHandlerContext->start ();

}

}
