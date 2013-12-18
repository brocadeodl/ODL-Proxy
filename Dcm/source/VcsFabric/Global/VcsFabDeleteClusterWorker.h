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
 *@file VcsFabDeleteClusterWorker.h 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc. All
 * rights reserved.
 * Description: This file ddeclares the
 *              Worker class which is responsible for processing
 *              Delete Cluster request received from the
 *              VcsFabricLocalObjectManager.
 *         
 *              
 * Author :     Aashish Akhouri
 * Date   :     11/16/2010
 *  
 */

#ifndef VCSFABDELETECLUSTERWORKER_H
#define VCSFABDELETECLUSTERWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Cluster/ClusterMessages.h"

using namespace WaveNs;

namespace DcmNs
{

//forward declarations

class VcsDeleteClusterMessage;
class VcsFabMessagingContext;


class VcsFabDeleteClusterWorker: public WaveWorker
{
public:
    VcsFabDeleteClusterWorker(WaveObjectManager* pWaveObjectManager);
    virtual ~VcsFabDeleteClusterWorker ();

protected:
   

private:
   //Message Handler for the Received received from the VcsFabricLocalObjectManager
   void deleteClusterMessageHandler (VcsDeleteClusterMessage *pVcsDeleteClusterMessage);

   //steps in the sequencer and call back
   void deleteVcsClusterManagedObjectStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void sendDeleteClusterMessageToClusterStep (VcsFabMessagingContext* pVcsFabMessagingContext);
   void sendDeleteClusterMessageCallBack (FrameworkStatus frameworkStatus,ClusterObjectManagerDeleteClusterMessage *pClusterObjectManagerDeleteClusterMessage, void *pContext);


   //message creation for Delete cluster message
   virtual PrismMessage *createMessageInstance (const UI32 &operationCode);
};

}

#endif //VCSFABDELETECLUSTERWORKER_H
