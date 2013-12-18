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
 *@file DistributedDebugGlobalObjectManager.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file declares the
 *              class which is responsible for sending debug
 *              queries and putting together all the responses
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */




#ifndef DISTRIBUTEDDEBUGGLOBALOBJECTMANAGER_H
#define DISTRIBUTEDDEBUGGLOBALOBJECTMANAGER_H


#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureMessages.h"
#include "Framework/ObjectModel/WaveObjectManager.h"



namespace WaveNs
{



class DistributedDebugGlobalObjectManager : public WaveObjectManager
{
public:

	        static  string          getClassName ();
                static  PrismServiceId  getPrismServiceId ();
                static  string          getPrismServiceName ();                
                static  DistributedDebugGlobalObjectManager *getInstance ();

                void  runDebugScriptMessageHandler  (RunDebugScriptMessage* pRunDebugScriptMessage);

private:
                DistributedDebugGlobalObjectManager ();
	        virtual ~DistributedDebugGlobalObjectManager ();

                virtual PrismMessage *createMessageInstance    (const UI32 &operationCode);

                void sendDebugScriptToAllNodesStep (PrismLinearSequencerContext* pRunDebugScriptMessageHandlerContext);

                void sendDebugScriptToAllNodesCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
};

}

#endif
