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
 * All rights reserved. Description: This file declares the
 *              class which is responsible for sending debug
 *              queries and putting together all the responses
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */
#ifndef DISTRIBUTEDDEBUGLOCALOBJECTMANAGER_H
#define DISTRIBUTEDDEBUGLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
namespace WaveNs
{


class PrismSynchronousLinearSequencerContext;
class RunDebugScriptOnClusterMemberMessage;

class DistributedDebugLocalObjectManager : public WaveLocalObjectManager
{
    public:
	static DistributedDebugLocalObjectManager       *getInstance                  ();
        static PrismServiceId                            getPrismServiceId                         (); 

        static string   getServiceName ();


    private:
                                   DistributedDebugLocalObjectManager                                      ();
    virtual                       ~DistributedDebugLocalObjectManager                                      ();


    virtual PrismMessage *createMessageInstance    (const UI32 &operationCode);


              void executeScriptStep (PrismSynchronousLinearSequencerContext *pRunDebugScriptMessageHandlerContext);

              void  runDebugScriptOnClusterMemberMessageHandler(RunDebugScriptOnClusterMemberMessage*  pRunDebugScriptOnClusterMemberMessage);

    // Now the data members

 
};

}

#endif
