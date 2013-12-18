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
 *@file DistributedDebugLocalObjectManager.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file declares the
 *              class which is responsible for running the debug
 *              script on each node. It then sends an outout as
 *              a strign back to global service
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */

#include "Framework/Utils/StringUtils.h"

#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureTypes.h" 
#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureMessages.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugLocalObjectManager.h" 
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Trace/TraceObjectManager.h"


using namespace std;
namespace WaveNs
{


/** 
 * 
 * 
 */
DistributedDebugLocalObjectManager::DistributedDebugLocalObjectManager ()
  :WaveLocalObjectManager(getServiceName ())
{
    addOperationMap(RUN_DEBUG_SCRIPT_ON_CLUSTER_MEMBER_MESSAGE, reinterpret_cast<PrismMessageHandler> (&DistributedDebugLocalObjectManager::runDebugScriptOnClusterMemberMessageHandler));

}

/** 
 * 
 * 
 */
DistributedDebugLocalObjectManager::~DistributedDebugLocalObjectManager ()
{

}

/** 
 * 
 * 
 * 
 * @return DistributedDebugLocalObjectManager*
 */
DistributedDebugLocalObjectManager* DistributedDebugLocalObjectManager::getInstance ()
{
    static DistributedDebugLocalObjectManager *pDistributedDebugLocalObjectManager = new DistributedDebugLocalObjectManager ();

    WaveNs::prismAssert (NULL != pDistributedDebugLocalObjectManager, __FILE__, __LINE__);

    return (pDistributedDebugLocalObjectManager);

}

/** 
 * 
 * 
 * 
 * @return PrismServiceId
 */
PrismServiceId   DistributedDebugLocalObjectManager::getPrismServiceId ()
{
     return ((getInstance ())->getServiceId ());

}

string DistributedDebugLocalObjectManager::getServiceName ()
{
  return ("Distributed Debug Infrastructure Local"); 
}


/** 
 * 
 * 
 * @param operationCode
 * 
 * @return PrismMessage*
 */
PrismMessage *DistributedDebugLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
       case RUN_DEBUG_SCRIPT_ON_CLUSTER_MEMBER_MESSAGE:

	    trace (TRACE_LEVEL_INFO, string ("DistributedDebugLocalObjectManager::createMessageInstance : operation code : ") + operationCode);
            pPrismMessage = new RunDebugScriptOnClusterMemberMessage(); 
            break;

        default :
            trace (TRACE_LEVEL_FATAL, string ("DistributedDebugLocalObjectManager::createMessageInstance : Unknown operation code : ") + operationCode);
            prismAssert (false, __FILE__, __LINE__);
            break;
    }

    return (pPrismMessage);
}

/** 
 * Name: executeScriptStep: 
 *  
 * Description: Execute the nos or bash script in the local 
 *              Service 
 * 
 * @param pRunDebugScriptMessageHandlerContext
 */
void DistributedDebugLocalObjectManager::executeScriptStep (PrismSynchronousLinearSequencerContext *pRunDebugScriptMessageHandlerContext )
{

   RunDebugScriptOnClusterMemberMessage*  pRunDebugScriptOnClusterMemberMessage = static_cast<RunDebugScriptOnClusterMemberMessage* > (pRunDebugScriptMessageHandlerContext->getPPrismMessage());

   FILE   *pPipe;
    
   string inputString = pRunDebugScriptOnClusterMemberMessage->getScriptString ();
   tracePrintf(TRACE_LEVEL_DEBUG, true, false, "DistributedDebugLocalObjectManager::executeScriptStep: inputString: %s", inputString.c_str());
   string outputString; 

   if(pRunDebugScriptOnClusterMemberMessage->isScriptNos ()==false) 
   {
       if( (pPipe = popen( inputString.c_str(), "r" )) == NULL )
       {
	    trace(TRACE_LEVEL_ERROR,"DistributedDebugLocalObjectManager::executeScriptStep: Failed to execute the bash script");
	    return;
       }


   }
   else 
   {

     trace(TRACE_LEVEL_DEBUG,"DistributedDebugLocalObjectManager::executeScriptStep:NOS Script ");

     string wyserEaCmdString = string("export ROLE_ID=root;export HOME=/root;export LOGNAME=root;export PWD=/root;/usr/WyserEa/bin/WyserEaCli -C << EOF\n");
     wyserEaCmdString+=inputString;
     wyserEaCmdString+=string ("\nEOF\n");

     if( (pPipe = popen( wyserEaCmdString.c_str(), "r" )) == NULL )
     {
	 trace(TRACE_LEVEL_ERROR,"DistributedDebugLocalObjectManager::executeScriptStep: Failed to execute the bash script");
	 return;
     }

   }

   while( !feof( pPipe ) )
   {
      char   psBuffer[128];
      memset(psBuffer,0,128);
      if( fgets( psBuffer, 128, pPipe ) != NULL )
      {
  	string tempString(psBuffer); 
  	outputString+=tempString;
      }
   }

   pclose( pPipe );

   tracePrintf(TRACE_LEVEL_DEBUG, true, false, "DistributedDebugLocalObjectManager::executeScriptStep: Final outputString: %s", outputString.c_str());
   pRunDebugScriptOnClusterMemberMessage->setOutputString(outputString);
}

/** 
 * Name: runDebugScriptOnClusterMemberMessageHandler
 * Description: Handler for message informing the Local Service 
 *              to execute the nos or bash script locally 
 * @param pRunDebugScriptOnClusterMemberMessage
 */
void  DistributedDebugLocalObjectManager::runDebugScriptOnClusterMemberMessageHandler(RunDebugScriptOnClusterMemberMessage*  pRunDebugScriptOnClusterMemberMessage)
{

    
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&DistributedDebugLocalObjectManager::executeScriptStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DistributedDebugLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DistributedDebugLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pRunDebugScriptMessageHandlerContext = new PrismSynchronousLinearSequencerContext (pRunDebugScriptOnClusterMemberMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pRunDebugScriptMessageHandlerContext->execute ();

}



}


